#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <memory>

#include <SFML/Window.hpp> // tmp
#include <SFML/System.hpp>
#include <enet/enet.h>

#include "core/EntityRegistry.hpp"
#include "core/Tasks.hpp"
#include "core/Error.hpp"
#include "core/Log.hpp"
#include "core/Time.hpp"
#include "input/ClockTimeSource.hpp"
#include "net/NetSystem.hpp"
#include "net/MessageHub.hpp"
#include "net/MessageTypes.hpp"
#include "net/Definitions.hpp"
#include "net/NetState.hpp"
#include "net/NetComponent.hpp"
#include "net/ENetReceiver.hpp"
#include "map/Map.hpp"
#include "world/TickSystem.hpp"
#include "world/PhysicsNetState.hpp"
#include "world/CircularMotion.hpp"
#include "world/MessageTypes.hpp"
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
        new CircularMotion(physics)
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
    sf::Clock clock; // tmp

    Tasks tasks;

    std::unique_ptr<MessageHub> messageHub;
    Clients clients;

    Map map;

    PhysicsSystem physicsSystem;
    ServerNetSystem netSystem;
    TickSystem tickSystem;
    ProjectileSystem projectileSystem;
    EntityRegistry entities;

    Tick tick; // tmp

    Server()
        : ENetReceiver(),
          messageHub(new MessageHub),
          clients(*messageHub.get()),
          physicsSystem(map),
          netSystem(clients),
          projectileSystem(map),
          entities({ &physicsSystem,
                     &netSystem,
                     &tickSystem,
                     &projectileSystem }),
          tick(1) {
        tasks.add(TICK_FREQUENCY, [&] () { runTick(); });

        createTestWorld();

        messageHub->onMessageWithPeer<PlayerInputMessage>([&]
        (ENetPeer* peer, PlayerInput const& playerInput) {
            auto client = ClientInfo::get(peer);
            auto entity = client->entity;
            if (entity) {
                auto input = entity->component<PlayerInputComponent>();
                auto physics = entity->component<PhysicsComponent>();
                ASSERT(input);
                ASSERT(physics);

                input->onPlayerInput(playerInput);

                if (playerInput.shoot) {
                    auto components =
                        makeProjectile(netSystem.makeNetEntityId(),
                                       0,
                                       physics->getPosition(),
                                       vec3(playerInput.orientation.x,
                                            0,
                                            playerInput.orientation.y));
                    entities.create(std::move(components));
                }

#ifdef USE_PREDICTION
                vec3 newPosition = physics->getPosition();
                messageHub->send<PlayerPositionMessage>(peer, newPosition);
#endif
            }
        });

        messageHub->onMessageWithPeer<DisconnectMessage>([&]
        (ENetPeer* peer) {
            auto client = ClientInfo::get(peer);
            handleDisconnect(client);
        });
    }

    ~Server() {
        if (host)
            enet_host_destroy(host);
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
        receive();

        tickSystem.tick();
        projectileSystem.tick(ProjectileComponent::GLOBAL);
        sendStates();

        ++tick;
    }

    void update(Time delta) {
        receive();
        tasks.run(delta);
    }

    void sendStates() const {
        for (auto& client : clients) {
            if (!client->connected)
                continue;
            
            BitStreamWriter stream;
            write(stream, tick);
#ifdef USE_PREDICTION
            netSystem.writeRawStates(stream, client->id);
#else
            netSystem.writeRawStates(stream);
#endif

            sendState(client.get(), stream);

            //TRACE(server) << "Sending state for tick #" << tick;
        }
    }

    void sendState(ClientInfo* client, BitStreamWriter const& stream) const {
        ENetPacket* packet = enet_packet_create(
                reinterpret_cast<void const*>(stream.ptr()),
                stream.size(),
                0);
        enet_peer_send(client->peer, 1, packet);
    }

    void handleDisconnect(ClientInfo* client) {
        if (!client->connected)
            return;

        INFO(server) << "Client " << *client << " disconnected";

        clients.broadcast<ClientDisconnectedMessage>(client->id);

        if (client->entity) {
            entities.remove(client->entity);
            client->entity = nullptr;
        }

        client->connected = false;
    }

protected:
    // Implement ENetReceiver
    void onConnect(ENetPeer* peer) {
        std::unique_ptr<ClientInfo> newClient(
                new ClientInfo(clients.makeClientId(), peer));
        ClientInfo* newClientPtr = newClient.get();
        
        INFO(server) << "Client " << (int)newClient->id << " connected";

        // Create player entity for the client
        newClient->entity = entities.create(
            makePlayer(netSystem.makeNetEntityId(),
                       newClient->id,
                       vec3(0, 0, 0),
                       vec3(0, 0, 0)));

        // It's important that the new client is registered only after
        // the client's player entity was created. Otherwise, the client
        // would be informed about its creation twice.
        clients.add(std::move(newClient));

        // Tell the client its id. This needs to happen before sending
        // the CreateEntityMessages, so that the client can identify what
        // entities belong to it.
        messageHub->send<LoggedInMessage>(peer, newClientPtr->id);

        // Send messages to create our net objects on the client-side
        netSystem.sendCreateEntityMessages(newClientPtr);

        // Tell everyone about the new client
        clients.broadcast<ClientConnectedMessage>(newClientPtr->id, "dummy");
    }

    void onDisconnect(ENetPeer* peer) {
        auto client = reinterpret_cast<ClientInfo*>(peer->data);
        handleDisconnect(client);
        clients.remove(client);
    }

    void onPacket(int channelID, ENetPeer* peer, ENetPacket* packet) {
        if (channelID == CHANNEL_MESSAGES)
            messageHub->dispatch(peer, packet);
        else
            ASSERT(false);
    }
};

int main() {
    Log::addSink(new ConsoleLogSink);
    Log::addSink(new FileLogSink("server.log"));

    initializeMessageTypes();

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
