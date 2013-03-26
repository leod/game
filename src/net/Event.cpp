#include "net/Event.hpp"

#include <boost/scoped_array.hpp>
#include <sstream>

#include "core/Error.hpp"
#include "core/Log.hpp"
#include "core/Event.hpp"
#include "util/BitStream.hpp"

namespace game {

WishBase::~WishBase() {

}

void emitEventFromPacket(EventHub const& hub, ENetPacket const* packet,
                         ClientId id) {
    BitStreamReader stream(packet->data, packet->dataLength); 
    
    // Read event type id and look up event type  
    EventTypeId typeId;
    read(stream, typeId);
    auto type = EventType::getById(typeId);

    // Create event storage and unserialize
    boost::scoped_array<uint8_t> data(new uint8_t[type.size]);
    type.init(data.get());

    auto event = reinterpret_cast<EventBase*>(data.get());
    event->read(stream);

    // Feel bad about yourself
    if (id != 0) {
        if (auto wish = dynamic_cast<WishBase*>(event))
            wish->clientId = id;
    }

    // Debug
    {
        std::stringstream ss;
        ss << *event;

        //TRACE(net) << "Received message " << ss.str();
    }

    // Emit event
    hub.emitBase(event);

    event->~EventBase();
}

void sendEvent(ENetPeer* peer, EventBase const& event) {
    // Serialize event into bitstream
    BitStreamWriter stream;
    write(stream, event.getType().id());
    event.write(stream);

    // Create enet packet and send off
    ENetPacket* packet = enet_packet_create(
           stream.ptr(), stream.size(), ENET_PACKET_FLAG_RELIABLE); 
    enet_peer_send(peer, CHANNEL_MESSAGES, packet);
}

} // namespace game
