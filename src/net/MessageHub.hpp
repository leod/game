#pragma once

#include <map>
#include <functional>
#include <vector>
#include <iostream>

#include <enet/enet.h>

#include "core/Error.hpp"
#include "util/BitStream.hpp"
#include "net/MessageType.hpp"

namespace game {

struct MessageHub {
    template<typename Message>
    void send(ENetPeer* peer, Message const& message) const {
        send(peer, message.toUntyped(), Message::type);
    }

    template<typename Message, typename... Args>
    void send(ENetPeer* peer, Args const&... args) const {
        Message message = Message::make(args...);
        send(peer, message.toUntyped(), Message::type);
    }

    template<typename Message, typename F>
    void onMessage(F f) {
        auto type = Message::type;
        auto dispatcher = [=]
        (ENetPeer*, UntypedMessage const* messageHandle) {
            auto message = static_cast<Message const*>(messageHandle); 
            message->unpack(f);
        };

        dispatchers.insert(std::make_pair(type.id(), dispatcher));
    }

    template<typename Message, typename F>
    void onMessageWithPeer(F f) {
        auto type = Message::type;
        auto dispatcher = [=]
        (ENetPeer* peer, UntypedMessage const* messageHandle) {
            auto message = static_cast<Message const*>(messageHandle); 
            message->unpack(f, peer);
        };

        dispatchers.insert(std::make_pair(type.id(), dispatcher));
    }

    void dispatch(ENetPeer*, ENetPacket*) const;

private:
    typedef std::function<void(ENetPeer*, UntypedMessage const*)> Dispatcher;
    std::multimap<MessageId, Dispatcher> dispatchers;

    void send(ENetPeer*, UntypedMessage const*, MessageType const&) const;
};

} // namespace game
