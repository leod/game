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

#include "server/ClientInfo.hpp"

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

    NetSystem netSystem;
    TickSystem tickSystem;
    EntityRegistry entities;

    ENetHost* host;
    std::vector<std::unique_ptr<ClientInfo>> clients;
    std::unique_ptr<MessageHub> messageHub;

    Tick tick; // tmp

    Server()
        : entities({ &netSystem, &tickSystem }),
          host(nullptr),
          clients(),
          messageHub(makeMessageHub()),
          tick(1),
          netEntityCounter(0) {
        tasks.add(TICK_FREQUENCY, [&] () { runTick(); });

        createTestWorld();

        messageHub->onMessageWithPeer<PlayerInputMessage>(
            [&] (ENetPeer* peer, PlayerInput const& input) {
                auto client = reinterpret_cast<ClientInfo*>(peer->data);
                ASSERT(client);

                client->inputComponent->onPlayerInput(input);
            });
    }

    ~Server() {
        if (host)
            enet_host_destroy(host);
    }

    void createTestWorld() {
        entities.add(makeTeapot(makeNetEntityId(), vec3(0, 0, 0)));
        entities.add(makeTeapot(makeNetEntityId(), vec3(-3, 0, 0)));
        entities.add(makeTeapot(makeNetEntityId(), vec3(3, 0, 0)));
    }

    void start() {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = 20000;

        host = enet_host_create(&address, 32, 2, 0, 0);

        if (host == nullptr)
            throw std::runtime_error("Failed to create host");
    }

    template<typename Message>
    void broadcast(Message const& message) {
        for (auto& client : clients)
            messageHub->send(client->peer, message);
    }

    template<typename Message, typename... Args>
    void broadcast(Args const&... args) {
        for (auto& client : clients)
            messageHub->send<Message>(client->peer, args...);
    }

    Entity* createEntity(ComponentList components) {
        Entity* entity = entities.add(components); 

        auto netComponent = entity->component<NetComponent>();
        ASSERT(netComponent);

        // TODO: Hack.
        vec3 position(0, 0, 0);
        if (auto physics = entity->component<PhysicsComponent>())
            position = physics->getPosition(); 

        broadcast<CreateEntityMessage>(netComponent->getNetTypeId(),
                                       netComponent->getNetId(),
                                       netComponent->getOwner(),
                                       position);

        return entity;
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

            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << "Client connected" << std::endl;

                std::unique_ptr<ClientInfo> newClient(new ClientInfo);
                newClient->peer = event.peer;
                newClient->id = makeClientId();

                auto player =
                    createEntity(makePlayer(makeNetEntityId(), newClient->id,
                                            vec3(0, 0, 0)));
                newClient->inputComponent =
                    player->component<PlayerInputComponent>();
                ASSERT(newClient->inputComponent);

                event.peer->data = reinterpret_cast<void*>(newClient.get());
                clients.push_back(std::move(newClient));

                messageHub->send<LoginMessage>(event.peer, clients.back()->id);

                netSystem.iterate([&] (NetComponent* component) {
                    // TODO: Hack.
                    vec3 position(0, 0, 0);
                    if (auto physics = component->getEntity()
                            ->component<PhysicsComponent>())
                        position = physics->getPosition(); 

                    messageHub->send<CreateEntityMessage>(event.peer,
                        component->getNetTypeId(),
                        component->getNetId(),
                        component->getOwner(),
                        position);
                });

                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected" << std::endl;
                break;

            default:
                ASSERT(false);
            }
        }
    }

    void runTick() {
        receive();

        tickSystem.tick();

        for (auto& client : clients) {
            BitStreamWriter stream;
            write(stream, tick);
            netSystem.writeRawStates(stream);

            sendState(*client.get(), stream);

            std::cout << "@" << clock.getElapsedTime().asMilliseconds()
                      << ": sending state " << tick << std::endl;
        }

        ++tick;
    }

    void update(Time delta) {
        receive();
        tasks.run(delta);
    }

    void sendState(ClientInfo& client, BitStreamWriter const& stream) {
        ENetPacket* packet = enet_packet_create(
                reinterpret_cast<void const*>(stream.ptr()),
                stream.size(),
                0);
        enet_peer_send(client.peer, 1, packet);
    }

    // For now
    NetEntityId netEntityCounter;
    NetEntityId makeNetEntityId() {
        return ++netEntityCounter;
    }

    ClientId makeClientId() const {
        for (ClientId test = 1; test < MAX_CLIENT_ID; ++test) {
            for (auto const& client : clients) {
                if (client->id == test)
                    goto continue_outer_loop; // Id already in use
            }

            // Id not in use
            return test;

continue_outer_loop:
            ;
        } 

        // No id found
        ASSERT_MSG(false, "Server accepted too many clients.");
    }
};

int main() {
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
