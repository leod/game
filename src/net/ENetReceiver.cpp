#include "net/ENetReceiver.hpp"

#include "core/Error.hpp"

namespace game {

ENetReceiver::ENetReceiver()
    : host(nullptr) {
}

void ENetReceiver::receive() {
#ifdef SIMULATE_LAG
    while (delayedPackets.size() > 0 && clock.getElapsedTime() >=
           delayedPackets.front().time) {
        auto dp = delayedPackets.front();
        onPacket(dp.channelID, dp.peer, dp.packet);
        enet_packet_destroy(dp.packet);
        delayedPackets.pop();
    }
#endif

    ENetEvent event;
    while (enet_host_service(host, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_RECEIVE: {
#ifdef SIMULATE_LAG
            auto time = clock.getElapsedTime() + milliseconds(SIMULATE_LAG);
            auto packet = DelayedPacket { time, event.channelID, event.peer,
                event.packet };
            delayedPackets.push(packet);
#else
            onPacket(event.channelID, event.peer, event.packet);  
            enet_packet_destroy(event.packet);
#endif
            break;
        }
        case ENET_EVENT_TYPE_CONNECT:
            onConnect(event.peer);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            onDisconnect(event.peer);
            break;
        default:
            ASSERT(false);
        }
    }
}

void ENetReceiver::onConnect(ENetPeer*) {}
void ENetReceiver::onDisconnect(ENetPeer*) {}
void ENetReceiver::onPacket(int, ENetPeer*, ENetPacket*) {}

} // namespace game

