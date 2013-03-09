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
#include "world/TickSystem.hpp"
#include "world/PhysicsNetState.hpp"
#include "world/CircularMotion.hpp"
#include "world/MessageTypes.hpp"
#include "world/PlayerInputComponent.hpp"
#include "physics/PhysicsComponent.hpp"

#include "server/Clients.hpp"
#include "server/ServerNetSystem.hpp"

using namespace game;

ComponentList makeTeapot(NetEntityId id, vec3 position) {
    auto physics = new PhysicsComponent(position);
    return {
        physics,
        new NetComponent(0, id, 0, { new PhysicsNetState(physics) }),
        new CircularMotion(physics)
    };
}

ComponentList makePlayer(NetEntityId id, ClientId owner, vec3 position) {
    auto physics = new PhysicsComponent(position);
    return {
        physics,
        new NetComponent(1, id, owner, { new PhysicsNetState(physics) }),
        new PlayerInputComponent(physics),
    };
}

// I'll split Client and Server up as soon as I'll find out what they do.
struct Server {
    sf::Clock clock; // tmp

    Tasks tasks;

    ENetHost* host;
    std::unique_ptr<MessageHub> messageHub;
    Clients clients;

    ServerNetSystem netSystem;
    TickSystem tickSystem;
    EntityRegistry entities;

    Tick tick; // tmp

    Server()
        : host(nullptr),
          messageHub(makeMessageHub()),
          clients(*messageHub.get()),
          netSystem(clients),
          entities({ &netSystem, &tickSystem }),
          tick(1) {
        tasks.add(TICK_FREQUENCY, [&] () { runTick(); });

        createTestWorld();

        messageHub->onMessageWithPeer<PlayerInputMessage>([&]
        (ENetPeer* peer, PlayerInput const& input) {
                auto client = ClientInfo::get(peer);
                client->entity->component<PlayerInputComponent>()
                              ->onPlayerInput(input);
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
                makeTeapot(netSystem.makeNetEntityId(), vec3(0, 0, 0)));
        entities.create(
                makeTeapot(netSystem.makeNetEntityId(), vec3(-3, 0, 0)));
        entities.create(
                makeTeapot(netSystem.makeNetEntityId(), vec3(3, 0, 0)));
    }

    void start() {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = 20000;

        host = enet_host_create(&address, 32, 2, 0, 0);

        if (host == nullptr)
            throw std::runtime_error("Failed to create host");
    }

    void receive() {
        ENetEvent event;

        while (enet_host_service(host, &event, 0) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                if (event.channelID == CHANNEL_MESSAGES)
                    messageHub->dispatch(event.peer, event.packet);
                else
                    ASSERT(false);

                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_CONNECT:
                handleConnect(event);
                break;

            case ENET_EVENT_TYPE_DISCONNECT: {
                auto client = reinterpret_cast<ClientInfo*>(event.peer->data);
                std::cout << "Client " << (int)client->id << " disconnected"
                          << std::endl;

                handleDisconnect(client);
                clients.remove(client);

                break;
            }

            default:
                ASSERT(false);
            }
        }
    }

    void runTick() {
        receive();

        tickSystem.tick();
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
            netSystem.writeRawStates(stream);

            sendState(client.get(), stream);

            TRACE(server) << "Sending state for tick #" << tick;
        }
    }

    void sendState(ClientInfo* client, BitStreamWriter const& stream) const {
        ENetPacket* packet = enet_packet_create(
                reinterpret_cast<void const*>(stream.ptr()),
                stream.size(),
                0);
        enet_peer_send(client->peer, 1, packet);
    }

    void handleConnect(ENetEvent const& event) {
        std::unique_ptr<ClientInfo> newClient(
                new ClientInfo(clients.makeClientId(), event.peer));
        ClientInfo* newClientPtr = newClient.get();
        
        INFO(server) << "Client " << (int)newClient->id << " connected";

        // Create player entity for the client
        newClient->entity = entities.create(
            makePlayer(netSystem.makeNetEntityId(),
                       newClient->id,
                       vec3(0, 0, 0)));

        // It's important that the new client is registered only after
        // the client's player entity was created. Otherwise, the client
        // would be informed about its creation twice.
        clients.add(std::move(newClient));

        // Tell the client its id. This needs to happen before sending
        // the CreateEntityMessages, so that the client can identify what
        // entities belong to it.
        messageHub->send<LoggedInMessage>(event.peer, newClientPtr->id);

        // Send messages to create our net objects on the client-side
        netSystem.sendCreateEntityMessages(newClientPtr);

        // Tell everyone about the new client
        clients.broadcast<ClientConnectedMessage>(newClientPtr->id, "dummy");
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
};

int main() {
    Log::addSink(new ConsoleLogSink);
    Log::addSink(new FileLogSink("server.log"));

    try {
        enet_initialize();

        Server server;
        server.start();

        ClockTimeSource time;
        Time deltaTime;

        while (true) {
            server.update(deltaTime);
            deltaTime = time.nextDelta();            

            sf::sleep(sf::milliseconds(5));
        }
    } catch(std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}
