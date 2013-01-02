#pragma once

#include <map>
#include <boost/scoped_array.hpp>

#include <enet/enet.h>

#include "util/BitStream.hpp"

namespace game {

struct MessageHub {
    // The constructor creates ids for the message types based on their order.
    // Note that, since the ids need to be equivalent on all peers,
    // the order in which the message types are passed to the constructor
    // of MessageHub needs to be canonical.
    template<typename... Messages>
    MessageHub()
        : structInfoToMsgType(makeInfoMap()),
          idToMsgType(makeIdMap()) {
    }

    template<typename Message>
    void send(ENetPeer* peer, Message const& message) const {
        send(peer, message.toUntyped(), lookupType(typeid(Message)));
    }

    template<typename Message, typename... Params>
    void onMessage(std::function<void(Params)> f) {
        auto type = lookupType(typeid(Message));

        dispatchers.insert(std::make_pair(type.id,
            [&] (UntypedMessage const* messageHandle) {
                Message const* message =
                    static_cast<Message const*>(messageHandle); 

                message->unpack(f);
            }
        ));
    }

    void dispatch(ENetPacket*) const;

private:
    MessageType const& lookupType(std::type_info const&) const; 
    void send(ENetPeer*, MessageType const&, UntypedMessage const*) const;

    struct MessageType {
        MessageId id;
        MessageTypeInfo const* ti;
    };

    typedef std::map<std::type_info const*, MessageType> InfoMap;
    InfoMap const structInfoToMsgType;

    typedef std::map<MessageId, MessageType> IdMap;
    IdMap const idToMsgType;

    typedef std::function<void(UntypedMessage const*)> Dispatcher;
    typedef std::multimap<MessageId, Dispatcher> DispatcherMap;
    DispatcherMap dispatchers;

    template<typename Message, typename... Messages>
    static InfoMap makeInfoMap(MessageId count = 0) {
        auto m = makeMap<Messages...>(count + 1);
        m[&typeid(Message)] = { count, &Message::typeInfo };

        return m;
    }

    static InfoMap makeInfoMap(MessageId) {
        return InfoMap();
    } 

    template<typename Message, typename... Messages>
    static IdMap makeIdMap(MessageId count = 0) {
        auto m = makeIdMap<Messages...>(count + 1);

        // Check that the two maps structInfoToMsgType and idToMsgType
        // agree on the id of the Message
        auto typeIt = structInfoToMsgType.lookup(&typeid(Message));
        ASSERT(typeIt != structInfoToMsgType.end());
        auto type = typeIt->second;
        ASSERT(type.id == count);

        m[count] = type;

        return m;
    }

    static IdMap makeIdMap(MessageId) {
        return IdMap();
    }
};

} // namespace game
