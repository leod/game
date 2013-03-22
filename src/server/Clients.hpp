#pragma once

#include <string>
#include <memory>
#include <vector>
#include <ostream>

#include <enet/enet.h>

#include "util/BitStream.hpp"
#include "net/Definitions.hpp"
#include "net/EventQueue.hpp"
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

    EventQueue eventQueue;

    template<typename E, typename... Args>
    void queueEvent(Tick tick, Args const&... args) {
        eventQueue.add<E>(tick, args...);
    }

    ClientInfo(ClientId, ENetPeer*);

    static ClientInfo* get(ENetPeer*);
};

std::ostream& operator<<(std::ostream&, ClientInfo const&);

struct Clients {
    typedef std::vector<std::unique_ptr<ClientInfo>> ClientList;

    Clients(Tick& tickRef, EventHub&);

    ClientInfo* add(ENetPeer*);
    ClientInfo* get(ClientId);
    void remove(ClientInfo*);

    typename ClientList::iterator begin();
    typename ClientList::iterator end();
    typename ClientList::const_iterator begin() const;
    typename ClientList::const_iterator end() const;

    Tick tick() const;

    // Queue an event to be sent to all clients with a tick
    template<typename E, typename... Args>
    void queueEvent(Tick tick, Args const&... args) {
        for (auto& client : clients)
            client->eventQueue.add<E>(tick, args...);
    }

    template<typename E, typename... Args>
    void queueEvent(Args const&... args) {
        for (auto& client : clients)
            client->eventQueue.add<E>(tickRef, args...);
    }

    // Immediately send an event to all clients
    template<typename E, typename... Args>
    void broadcast(Args const&... args) const {
        for (auto& client : clients) {
            if (!client->connected) continue;
            sendEvent<E>(client->peer, args...);
        }
    }

private:
    Tick& tickRef;

    EventHub& eventHub;
    ClientList clients;
    
    // Find an id that is unique among those clients already registered.
    // Note that two subsequent calls will return the same id, unless a
    // new client is added inbetween.
    ClientId makeClientId() const;

};

} // namespace game
