#pragma once

#include "net/NetSystem.hpp"

namespace game {

struct Clients;
struct ClientInfo;

// Adds to NetSystem by adding code to broadcast to all clients
// messages to create and remove entities.
struct ServerNetSystem : NetSystem {
    ServerNetSystem(Clients&);

    // Override NetSystem
    void onRegister(NetComponent*); 
    void onUnregister(NetComponent*); 

    NetEntityId makeNetEntityId();

    void sendCreateEntityMessages(ClientInfo* const) const;

private:
    Clients& clients;

    NetEntityId netEntityCounter;
};

} // namespace game
