#pragma once

#include <string>
#include <memory>
#include <vector>
#include <ostream>

#include <enet/enet.h>

#include "net/Definitions.hpp"
#include "net/Event.hpp"

namespace game {

struct Entity;
struct EventHub;

struct ClientInfo {
    ClientId const id;
    ENetPeer* const peer;

    std::string name;
    Entity* entity;
    bool connected;

    ClientInfo(ClientId, ENetPeer*);

    static ClientInfo* get(ENetPeer*);
};

std::ostream& operator<<(std::ostream&, ClientInfo const&);

struct Clients {
    typedef std::vector<std::unique_ptr<ClientInfo>> ClientList;

    Clients(EventHub&);

    ClientInfo* add(ENetPeer*);
    ClientInfo* get(ClientId);
    void remove(ClientInfo*);

    typename ClientList::iterator begin();
    typename ClientList::iterator end();
    typename ClientList::const_iterator begin() const;
    typename ClientList::const_iterator end() const;

    // Send a message to all clients
    template<typename E>
    void broadcast(E const& event) const {
        for (auto& client : clients) {
            if (!client->connected) continue;
            sendEvent(client->peer, event);
        }
    }

    template<typename E, typename... Args>
    void broadcast(Args const&... args) const {
        for (auto& client : clients) {
            if (!client->connected) continue;
            sendEvent<E>(client->peer, args...);
        }
    }

private:
    EventHub& eventHub;
    ClientList clients;
    
    // Find an id that is unique among those clients already registered.
    // Note that two subsequent calls will return the same id, unless a
    // new client is added inbetween.
    ClientId makeClientId() const;

};

} // namespace game
