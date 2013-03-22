#include "server/Clients.hpp"

#include "core/Error.hpp"
#include "core/Event.hpp"

namespace game {

ClientInfo::ClientInfo(ClientId id, ENetPeer* peer)
    : id(id), peer(peer),
      name(""), entity(nullptr), connected(true) {
    peer->data = this;
}

ClientInfo* ClientInfo::get(ENetPeer* peer) {
    ASSERT(peer->data);
    return reinterpret_cast<ClientInfo*>(peer->data);
}

std::ostream& operator<<(std::ostream& os, ClientInfo const& client) {
    os << client.name << " (#" << (int)client.id << ")";
    return os;
}

Clients::Clients(Tick& tickRef, EventHub& eventHub)
    : tickRef(tickRef), eventHub(eventHub) {
}

ClientInfo* Clients::add(ENetPeer* peer) {
    auto clientId = makeClientId();

#ifndef NDEBUG
    // Assert that the id of the new client is unique
    for (auto& c : clients)
        ASSERT_MSG(clientId != c->id,
                   "Client id " << clientId <<
                   " already in use by " << c->name);
#endif

    clients.push_back(std::unique_ptr<ClientInfo>(
            new ClientInfo(clientId, peer))); 
    return clients.back().get();
}

ClientInfo* Clients::get(ClientId id) {
    for (auto& c : clients)
        if (c->id == id)
            return c.get();

    ASSERT_MSG(false, "Client #" << id << " not found");
}

void Clients::remove(ClientInfo* client) {
    ASSERT(client);

    auto it = clients.begin();
    for (; it != clients.end(); ++it)
        if ((*it)->id == client->id) break;

    ASSERT(it != clients.end());
    clients.erase(it);
}

typename Clients::ClientList::iterator Clients::begin() {
    return clients.begin();
}

typename Clients::ClientList::iterator Clients::end() {
    return clients.end();
}

typename Clients::ClientList::const_iterator Clients::begin() const {
    return clients.begin();
}

typename Clients::ClientList::const_iterator Clients::end() const {
    return clients.end();
}

Tick Clients::tick() const {
    return tickRef;
}

ClientId Clients::makeClientId() const {
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

} // namespace game
