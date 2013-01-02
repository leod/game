#include "net/MessageHub.hpp"

namespace game {

void MessageHub::dispatch(ENetPacket* packet) {
    BitStreamReader stream(packet->data, packet->dataLength);

    // Read message id
    MessageId id;
    read(stream, id);

    // Look up message type
    auto typeIt = idToMsgType.find(id);
    if (typeIt == idToMsgType.end())
        throw std::runtime_error("Received message of unknown type.");

    auto type = typeIt->second;

    // Create package storage and unserialize
    boost::scoped_array<uint8_t> data(new uint8_t[type->size]);
    auto message = static_cast<UntypedMessage*>(data.get());

    type.ti->read(stream, message);

    // Call corresponding dispatchers
    auto range = dispatchers.equal_range(id);
    for (auto it = range.first; it != range.second; it++)
        it->second(message);
}

MessageType const&
MessageHub::lookupType(std::type_info const& typeInfo) const {
    auto typeIt = structInfoToMsgType.find(&typeInfo);

    if(typeInfoIt != structInfoToMsgType.end())
        throw std::runtime_error(std::string("Message type ") +
                                 typeid(Message).name() +
                                 "has not been registered.");

    auto type = typeIt->second;

    return type;
}

void MessageHub::send(ENetPeer* peer, MessageType const& type,
                      UntypedMessage const* message) const {
    // Serialize message into a bitstream
    BitStreamWriter stream; 
    write(stream, type.id); 
    type.ti->write(stream, message.toUntyped());

    // Create enet packet and send off
    ENetPacket* packet = enet_packet_create(
            stream.ptr(), stream.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_packet_destroy(packet);
}

} // namespace game
