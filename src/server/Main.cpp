#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <memory>

#include <SFML/System.hpp>
#include <enet/enet.h>

#include "core/EntityManager.hpp"
#include "core/Tasks.hpp"
#include "core/Error.hpp"
#include "core/Log.hpp"
#include "core/Time.hpp"
#include "core/Event.hpp"

#include "util/Profiling.hpp"
#include "input/ClockTimeSource.hpp"
#include "net/NetSystem.hpp"
#include "net/Definitions.hpp"
#include "net/NetState.hpp"
#include "net/NetComponent.hpp"
#include "net/ENetReceiver.hpp"
#include "map/Map.hpp"
#include "world/TickSystem.hpp"
#include "world/PhysicsNetState.hpp"
#include "world/CircularMotion.hpp"
#include "world/EventTypes.hpp"
#include "world/PlayerInputComponent.hpp"
#include "world/ProjectileComponent.hpp"
#include "world/ProjectileSystem.hpp"
#include "world/PlayerComponent.hpp"
#include "physics/PhysicsComponent.hpp"
#include "physics/PhysicsSystem.hpp"

#include "server/Clients.hpp"
#include "server/ServerNetSystem.hpp"

using namespace game;

ComponentList makeTeapot(NetEntityId id, vec3 position, vec3 orientation) {
    auto physics = new PhysicsComponent(position, orientation);
    return {
        physics,
        new NetComponent(0, id, 0, { new PhysicsNetState(physics) }),
        new CircularMotion(physics),
        new PlayerComponent(nullptr)
    };
}

ComponentList makePlayer(NetEntityId id, ClientId owner, vec3 position,
                         vec3 orientation) {
    auto physics = new PhysicsComponent(position, orientation);
    return {
        physics,
        new NetComponent(1, id, owner, { new PhysicsNetState(physics) }),
        new PlayerInputComponent(physics),
        new PlayerComponent(nullptr)
    };
}

ComponentList makeProjectile(NetEntityId id, ClientId owner, vec3 position,
                             vec3 orientation) {
    auto physics = new PhysicsComponent(position, orientation);

    return {
        physics,
        new NetComponent(2, id, owner, { new PhysicsNetState(physics) }),
        new ProjectileComponent(physics, ProjectileComponent::GLOBAL)
    };
}

struct Server : public ENetReceiver {
    sf::Clock clock;

    Tasks tasks;
    EventHub eventHub;

    Clients clients;

    Map map;

    PhysicsSystem physicsSystem;
    ServerNetSystem netSystem;
    TickSystem tickSystem;
    ProjectileSystem projectileSystem;
    EntityManager entities;

    Tick tick;

    Server()
        : ENetReceiver(),
          clients(tick, eventHub),
          physicsSystem(map),
          netSystem(eventHub, clients),
          projectileSystem(map),
          entities({ &physicsSystem,
                     &netSystem,
                     &tickSystem,
                     &projectileSystem }),
          tick(1) {
        tasks.add(TICK_FREQUENCY, [&] () { runTick(); });
        tasks.add(0.1, [&] () { ProfilingData::dump(); });

        eventHub.subscribe<PlayerInputWish>(this, &Server::onPlayerInputWish);
        eventHub.subscribe<DisconnectWish>(this, &Server::onDisconnectWish);

        eventHub.subscribe<Ping>(this, &Server::onPing);

        createTestWorld();
    }

    ~Server() {
        if (host)
            enet_host_destroy(host);
    }

    void onPlayerInputWish(ClientId clientId, PlayerInput const& playerInput) {
        auto client = clients.get(clientId);

        auto entity = client->entity;
        if (entity) {
            auto input = entity->component<PlayerInputComponent>();
            auto physics = entity->component<PhysicsComponent>();
            ASSERT(input);
            ASSERT(physics);

            input->onPlayerInput(playerInput);

            if (playerInput.shoot) {
                auto origin = physics->getPosition() +
                              physics->getOrientation() * 0.2f;
                auto orientation =
                    vec3(playerInput.orientation.x, 0,
                         playerInput.orientation.y);
                auto components =
                    makeProjectile(netSystem.makeNetEntityId(),
                                   clientId,
                                   origin,
                                   orientation);
                entities.create(std::move(components));
            }

#ifdef USE_PREDICTION
            vec3 newPosition = physics->getPosition();
            sendEvent<PlayerPositionOrder>(client->peer, newPosition);
#endif
        }
    }

    void onDisconnectWish(ClientId clientId) {
        handleDisconnect(clients.get(clientId));
    }

    void onPing(ClientId clientId) {
        sendEvent<Pong>(clients.get(clientId)->peer);
    }

    void createTestWorld() {
        entities.create(
                makeTeapot(netSystem.makeNetEntityId(), vec3(0, 0, 0),
                           vec3(0, 0, 0)));
        entities.create(
                makeTeapot(netSystem.makeNetEntityId(), vec3(-3, 0, 0),
                           vec3(0, 0, 0)));
        entities.create(
                makeTeapot(netSystem.makeNetEntityId(), vec3(3, 0, 0),
                           vec3(0, 0, 0)));
    }

    void start() {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = 20000;

        host = enet_host_create(&address, 32, 2, 0, 0);

        if (host == nullptr)
            throw std::runtime_error("Failed to create host");
    }

    void runTick() {
        PROFILE(runTick);

        receive();

        tickSystem.tick();
        {
            PROFILE(projectiles);
            projectileSystem.tick(ProjectileComponent::GLOBAL);
        }

        sendTicks();

        ++tick;
    }

    void update(Time delta) {
        PROFILE(update);

        {
            PROFILE(receive);
            receive();
        }
        tasks.run(delta);
    }

    void sendTicks() const {
        PROFILE(sendTicks);

        for (auto& client : clients) {
            if (!client->connected)
                continue;

            BitStreamWriter stream;
            write(stream, tick);

            while (!client->eventQueue.empty() &&
                   client->eventQueue.frontTick() <= tick)
                client->eventQueue.popAndWrite(stream);

            {
                EventTypeId zero = 0;
                write(stream, zero);
            }

#ifdef USE_PREDICTION
            netSystem.writeStates(stream, client->id);
#else
            netSystem.writeStates(stream);
#endif

            //TRACE(server) << "Sending state for tick #" << tick;
            sendTick(client.get(), stream);
        }
    }

    void sendTick(ClientInfo* client, BitStreamWriter const& stream) const {
        ENetPacket* packet = enet_packet_create(
                reinterpret_cast<void const*>(stream.ptr()),
                stream.size(),
                ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(client->peer, 1, packet);
    }

    void handleDisconnect(ClientInfo* client) {
        if (!client->connected)
            return;

        INFO(server) << "Client " << *client << " disconnected";

        clients.broadcast<ClientDisconnectedOrder>(client->id);

        if (client->entity) {
            entities.remove(client->entity);
            client->entity = nullptr;
        }

        client->connected = false;
    }

protected:
    // Implement ENetReceiver
    void onConnect(ENetPeer* peer) {
        auto newClient = clients.add(peer);

        INFO(server) << "Client " << (int)newClient->id << " connected";

        // Tell the client its id. This needs to happen before sending
        // the CreateEntityMessages, so that the client can identify what
        // entities belong to it.
        sendEvent<LoggedInOrder>(peer, newClient->id);
        
        // Create player entity for the client
        newClient->entity = entities.create(
            makePlayer(netSystem.makeNetEntityId(),
                       newClient->id,
                       vec3(0, 0, 0),
                       vec3(0, 0, 0)));

        // Send messages to create our net objects on the client side
        netSystem.sendCreateEntityOrders(newClient);

        // Tell everyone about the new client
        clients.broadcast<ClientConnectedOrder>(newClient->id, "dummy");
    }

    void onDisconnect(ENetPeer* peer) {
        auto client = reinterpret_cast<ClientInfo*>(peer->data);
        handleDisconnect(client);
        clients.remove(client);
    }

    void onPacket(int channelId, ENetPeer* peer, ENetPacket* packet) {
        if (channelId == CHANNEL_MESSAGES)
            emitEventFromPacket(eventHub, packet, ClientInfo::get(peer)->id);
        else
            ASSERT(false);
    }
};

int main() {
    Log::addSink(new ConsoleLogSink);
    Log::addSink(new FileLogSink("server.log"));

    initializeEventTypes();

    try {
        enet_initialize();

        Server server;
        server.start();

        ClockTimeSource time;
        Time deltaTime;

        while (true) {
            server.update(deltaTime);
            deltaTime = time.nextDelta();            

            sf::sleep(sf::milliseconds(0));
        }
    } catch(std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}
