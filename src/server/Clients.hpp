#pragma once

#include <string>
#include <memory>
#include <vector>

#include <enet/enet.h>

#include "net/Definitions.hpp"
#include "net/MessageHub.hpp"

namespace game {

struct Entity;

struct ClientInfo {
    ClientId const id;
    ENetPeer* const peer;

    std::string name;
    Entity* entity;
    bool connected;

    ClientInfo(ClientId, ENetPeer*);

    static ClientInfo* get(ENetPeer*);
};

struct Clients {
    typedef std::vector<std::unique_ptr<ClientInfo>> ClientList;

    Clients(MessageHub&);

    MessageHub& getMessageHub();
    MessageHub const& getMessageHub() const;

    void add(std::unique_ptr<ClientInfo>&&);
    void remove(ClientInfo*);

    typename ClientList::iterator begin();
    typename ClientList::iterator end();
    typename ClientList::const_iterator begin() const;
    typename ClientList::const_iterator end() const;

    // Find an id that is unique among those clients already registered.
    // Note that two subsequent calls will return the same id, unless you
    // add a new client inbetween.
    ClientId makeClientId() const;

    // Send a message to all clients
    template<typename Message>
    void broadcast(Message const& message) const {
        for (auto& client : clients) {
            if (!client->connected) continue;
            messageHub.send(client->peer, message);
        }
    }

    template<typename Message, typename... Args>
    void broadcast(Args const&... args) const {
        for (auto& client : clients) {
            if (!client->connected) continue;
            messageHub.send<Message>(client->peer, args...);
        }
    }

private:
    MessageHub& messageHub;
    ClientList clients;
};

} // namespace game
