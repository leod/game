#include "server/Clients.hpp"

#include "core/Error.hpp"

namespace game {

ClientInfo::ClientInfo(ClientId id, ENetPeer* peer)
    : id(id), peer(peer),
      name(), entity(nullptr), connected(true) {
}

Clients::Clients(MessageHub& messageHub)
    : messageHub(messageHub) {
}

MessageHub& Clients::getMessageHub() {
    return messageHub;
}

MessageHub const& Clients::getMessageHub() const {
    return messageHub;
}

void Clients::add(std::unique_ptr<ClientInfo>&& client) {
#ifndef NDEBUG
    // Assert that the id of the new client is unique
    for (auto& c : clients)
        ASSERT_MSG(client->id != c->id,
                   "Client id " << c->id << " already in use by " << c->name);
#endif

    clients.push_back(std::move(client)); 
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
