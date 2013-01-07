#include "net/MessageHub.hpp"

#include <stdexcept>
#include <sstream>
#include <boost/scoped_array.hpp>

namespace game {

void MessageHub::dispatch(ENetPeer* peer, ENetPacket* packet) const {
    BitStreamReader stream(packet->data, packet->dataLength);

    // Read message id
    MessageId id;
    read(stream, id);

    // Look up message type
    auto typeIt = idMap.find(id);
    if (typeIt == idMap.end()) {
        std::stringstream error;
        error << "Received message of unknown type: ";
        error << id;
        error << ".";

        throw std::runtime_error(error.str());
    }

    auto type = typeIt->second;

    // Create packet storage and unserialize
    boost::scoped_array<uint8_t> data(new uint8_t[type.ti->size]);
    auto message = static_cast<UntypedMessage*>(data.get());

    type.ti->read(stream, message);

    // Call corresponding dispatchers
    auto range = dispatchers.equal_range(id);
    for (auto it = range.first; it != range.second; it++)
        it->second(peer, message);
}

MessageHub::MessageHub(MessageTypeInfoVector const& types)
    : typeInfoMap(makeTypeInfoMap(types)), idMap(makeIdMap(types)) {
}

MessageHub::NamedMessageType const&
MessageHub::lookupType(std::type_info const& typeInfo) const {
    auto typeIt = typeInfoMap.find(&typeInfo);

    if(typeIt == typeInfoMap.end())
        throw std::runtime_error(std::string("Message type ") +
                                 typeInfo.name() +
                                 "has not been registered.");

    return typeIt->second;
}

void MessageHub::send(ENetPeer* peer, UntypedMessage const* message,
                      NamedMessageType const& type) const {
    // Serialize message into a bitstream
    BitStreamWriter stream; 
    write(stream, type.id); 
    type.ti->write(stream, message);

    // Create enet packet and send off
    ENetPacket* packet = enet_packet_create(
            stream.ptr(), stream.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

MessageHub::TypeInfoMap
MessageHub::makeTypeInfoMap(MessageTypeInfoVector const& types) {
    TypeInfoMap m; 
    for (size_t i = 0; i < types.size(); ++i) {
        m[types[i].first] = { static_cast<MessageId>(i), types[i].second };
    }
    return m;
}

MessageHub::IdMap MessageHub::makeIdMap(MessageTypeInfoVector const& types) {
    IdMap m; 
    for (size_t i = 0; i < types.size(); ++i) {
        m[i] = { static_cast<MessageId>(i), types[i].second };
    }
    return m;
}

} // namespace game
