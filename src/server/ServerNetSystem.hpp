#pragma once

#include "net/NetSystem.hpp"

namespace game {

struct EventHub;
struct Clients;
struct ClientInfo;

// Adds to NetSystem by adding code to broadcast to all clients
// messages to create and remove entities.
struct ServerNetSystem : NetSystem {
    ServerNetSystem(EventHub&, Clients&);

    // Override NetSystem
    void onRegister(NetComponent*); 
    void onUnregister(NetComponent*); 

    NetEntityId makeNetEntityId();

    void sendCreateEntityOrders(ClientInfo* const) const;

private:
    Clients& clients;

    NetEntityId netEntityCounter;
};

} // namespace game
