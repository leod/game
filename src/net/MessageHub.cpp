#include "net/MessageHub.hpp"

#include <stdexcept>

namespace game {

void MessageHub::dispatch(ENetPeer* peer, ENetPacket* packet) const {
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
    boost::scoped_array<uint8_t> data(new uint8_t[type.ti->size]);
    auto message = static_cast<UntypedMessage*>(data.get());

    type.ti->read(stream, message);

    // Call corresponding dispatchers
    auto range = dispatchers.equal_range(id);
    for (auto it = range.first; it != range.second; it++)
        it->second(peer, message);
}

MessageHub::MessageType const&
MessageHub::lookupType(std::type_info const& typeInfo) const {
    auto typeIt = structInfoToMsgType.find(&typeInfo);

    if(typeIt == structInfoToMsgType.end())
        throw std::runtime_error(std::string("Message type ") +
                                 typeInfo.name() +
                                 "has not been registered.");

    return typeIt->second;
}

void MessageHub::send(ENetPeer* peer, MessageType const& type,
                      UntypedMessage const* message) const {
    // Serialize message into a bitstream
    BitStreamWriter stream; 
    write(stream, type.id); 
    type.ti->write(stream, message);

    // Create enet packet and send off
    ENetPacket* packet = enet_packet_create(
            stream.ptr(), stream.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_packet_destroy(packet);
}

} // namespace game
