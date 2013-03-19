#pragma once

#include <enet/enet.h>

#include "core/Event.hpp"
#include "util/Tuple.hpp"
#include "net/Definitions.hpp"

#define DEFINE_ORDER(_name, ...) \
    DEFINE_EVENT_WITH_BASE(::game::Order, _name, ##__VA_ARGS__)
#define DEFINE_WISH(_name, ...) \
    DEFINE_EVENT_WITH_BASE(::game::Wish, _name, ##__VA_ARGS__)

namespace game {

struct EventHub;

template<typename E, typename... Args>
struct Order : public Event<E, Args...> {
};

struct WishBase {
    virtual ~WishBase();
    ClientId clientId;
};

template<typename E, typename... Types>
struct Wish : public Event<E, Types...>, public WishBase {
    // We modify Event's unpack function by adding the id
    // of the client as the first parameter
    template<typename F, typename... Params>
    void unpack(F f, Params const&... params) const {
        auto m = static_cast<Event<E, Types...> const*>(this)->m;
        tuple::Unpack<Types...>(m).call(f, clientId, params...);
    }
};

void emitEventFromPacket(EventHub const&, ENetPacket const*, ClientId id = 0);

template<typename E, typename... Types>
void sendEvent(ENetPeer* peer, Types const&... args) {
    E event = E::make(args...);
    sendEvent(peer, event);
}

void sendEvent(ENetPeer*, EventBase const&);

} // namespace game
