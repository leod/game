#pragma once

#include <queue>

#include <enet/enet.h>

#include "core/Time.hpp"
#include "net/Definitions.hpp"

namespace game {

struct ENetReceiver {
    ENetReceiver();

    void receive();

protected:
    ENetHost* host;

    virtual void onConnect(ENetPeer*);
    virtual void onDisconnect(ENetPeer*);
    virtual void onPacket(int channelId, ENetPeer*, ENetPacket*);

#ifdef SIMULATE_LAG
    struct DelayedPacket {
        Time time;

        int channelId;
        ENetPeer* peer;
        ENetPacket* packet;
    };

    Clock clock;
    std::queue<DelayedPacket> delayedPackets;
#endif
};

} // namespace game
