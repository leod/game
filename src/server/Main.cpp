#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <memory>

#include <enet/enet.h>

#include "core/EntityRegistry.hpp"
#include "core/Tasks.hpp"
#include "core/Error.hpp"
#include "net/NetSystem.hpp"
#include "net/MessageHub.hpp"
#include "net/Definitions.hpp"
#include "world/TickSystem.hpp"

#include "server/ClientInfo.hpp"

using namespace game;

// I'll split Client and Server up as soon as I'll find out what they do.
struct Server {
    Tasks tasks;

    NetSystem netSystem;
    TickSystem tickSystem;
    EntityRegistry entities;

    ENetHost* host;
    std::vector<ClientInfo> clients;
    std::unique_ptr<MessageHub> messageHub;

    Tick tick; // tmp

    Server()
        : entities({ &netSystem, &tickSystem }),
          host(nullptr),
          clients(),
          messageHub(MessageHub::make<>()),
          tick(0) {
        tasks.add(TICK_FREQUENCY, [&] () { runTick(); });
    }

    ~Server() {
        if (host)
            enet_host_destroy(host);
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
                std::cout << "Client connected" << std::endl;

                {
                    ClientInfo newClient;
                    newClient.peer = event.peer;
                    newClient.id = makeClientId();

                    clients.push_back(newClient);
                }

                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected" << std::endl;
                break;

            default:
                ASSERT(false);
            }
        }
    }

    void runTick() {
        tickSystem.tick();

        for (auto& client : clients) {
            BitStreamWriter stream;
            write(stream, tick);
            netSystem.writeRawStates(stream);

            sendState(client, stream);
        }

        tick++;
    }

    void sendState(ClientInfo& client, BitStreamWriter const& stream) {
        ENetPacket* packet = enet_packet_create(
                reinterpret_cast<void const*>(stream.ptr()),
                stream.size(),
                0);
        enet_peer_send(client.peer, 1, packet);
        enet_packet_destroy(packet);
    }

    ClientId makeClientId() const {
        for (ClientId test = 0; test < MAX_CLIENT_ID; ++test) {
            for (auto const& client : clients) {
                if (client.id == test)
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
    } catch(std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}
