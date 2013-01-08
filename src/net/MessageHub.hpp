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

typedef std::vector<std::pair<std::type_info const*, MessageType const*>>
        MessageTypeInfoVector;

template<typename Message, typename... Messages>
void makeMessageTypeInfoVector(MessageTypeInfoVector&);

struct MessageHub {
    // The constructor creates ids for the message types based on their order.
    // Note that, since the ids need to be equivalent on all peers,
    // the order in which the message types are passed to the constructor
    // of MessageHub needs to be canonical.
    template<typename... Messages>
    static MessageHub* make() {
        MessageTypeInfoVector typeInfos;
        makeMessageTypeInfoVector<Messages..., void>(typeInfos);
        return new MessageHub(typeInfos);
    }

    template<typename Message>
    void send(ENetPeer* peer, Message const& message) const {
        send(peer, message.toUntyped(), lookupType(typeid(Message)));
    }

    template<typename Message, typename... Args>
    void send(ENetPeer* peer, Args const&... args) const {
        Message message = Message::make(args...);
        send(peer, message.toUntyped(), lookupType(typeid(Message)));
    }

    template<typename Message, typename F>
    void onMessage(F f) {
        auto type = lookupType(typeid(Message));

        dispatchers.insert(std::make_pair(type.id,
            [=] (ENetPeer*, UntypedMessage const* messageHandle) {
                Message const* message =
                    static_cast<Message const*>(messageHandle); 

                message->unpack(f);
            }
        ));
    }

    template<typename Message, typename F>
    void onMessageWithPeer(F f) {
        auto type = lookupType(typeid(Message));

        dispatchers.insert(std::make_pair(type.id,
            [=] (ENetPeer* peer, UntypedMessage const* messageHandle) {
                Message const* message =
                    static_cast<Message const*>(messageHandle); 

                message->unpack(f, peer);
            }
        ));
    }

    void dispatch(ENetPeer*, ENetPacket*) const;

private:
    struct NamedMessageType {
        MessageId id;
        MessageType const* ti;
    };
    typedef std::map<std::type_info const*, NamedMessageType> TypeInfoMap;
    typedef std::map<MessageId, NamedMessageType> IdMap;
    typedef std::function<void(ENetPeer*, UntypedMessage const*)> Dispatcher;

    MessageHub(MessageTypeInfoVector const&);

    NamedMessageType const& lookupType(std::type_info const&) const; 
    void send(ENetPeer*, UntypedMessage const*, NamedMessageType const&) const;

    static TypeInfoMap makeTypeInfoMap(MessageTypeInfoVector const&);
    static IdMap makeIdMap(MessageTypeInfoVector const&);

    TypeInfoMap const typeInfoMap;
    IdMap const idMap;
    std::multimap<MessageId, Dispatcher> dispatchers;
};

template<typename Message, typename... Messages>
inline void makeMessageTypeInfoVector(MessageTypeInfoVector& vector) {
    vector.push_back(std::make_pair(&typeid(Message), &Message::typeInfo));
    makeMessageTypeInfoVector<Messages...>(vector);
}

template<>
inline void makeMessageTypeInfoVector<void>(MessageTypeInfoVector&) {
}

} // namespace game
