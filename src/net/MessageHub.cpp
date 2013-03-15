#include "net/MessageHub.hpp"

#include <stdexcept>
#include <sstream>
#include <boost/scoped_array.hpp>

#include "core/Log.hpp"

namespace game {

void MessageHub::dispatch(ENetPeer* peer, ENetPacket* packet) const {
    BitStreamReader stream(packet->data, packet->dataLength);

    // Read message id and lookup message type
    MessageId id;
    read(stream, id);

    auto type = MessageType::getById(id);

    // Create packet storage and unserialize
    boost::scoped_array<uint8_t> data(new uint8_t[type.size]);
    auto message = static_cast<UntypedMessage*>(data.get());

    type.init(message);
    type.read(stream, message);

    // Call corresponding dispatchers
    auto range = dispatchers.equal_range(id);
    for (auto it = range.first; it != range.second; it++)
        it->second(peer, message);

    TRACE(net) << "Received message " << type.toString(message);
}

void MessageHub::send(ENetPeer* peer, UntypedMessage const* message,
                      MessageType const& type) const {
    TRACE(net) << "Sending " << type.toString(message);

    // Serialize message into a bitstream
    BitStreamWriter stream; 
    write(stream, type.id()); 
    type.write(stream, message);

    // Create enet packet and send off
    ENetPacket* packet = enet_packet_create(
            stream.ptr(), stream.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

} // namespace game
