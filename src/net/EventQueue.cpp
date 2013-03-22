#include "net/EventQueue.hpp"

#include <sstream>

#include "core/Event.hpp"
#include "core/Error.hpp"
#include "util/BitStream.hpp"

namespace game {

bool EventQueue::addFromStream(Tick tick, BitStreamReader& stream) {
    EventTypeId typeId;
    read(stream, typeId);

    if (typeId == 0) return false;

    auto type = EventType::getById(typeId);
    EventBase* event = type.create();
    event->read(stream);

    add(tick, event);

    {
        std::stringstream ss;
        ss << *event;
        //TRACE(net) << "Add event to queue for tick " << tick << ": "
                   //<< ss.str();
    }

    return true;
}

bool EventQueue::empty() {
    return events.empty();
}

Tick EventQueue::frontTick() {
    ASSERT(!empty());
    return events.front().first;
} 

void EventQueue::popAndEmit(EventHub& hub) {
    ASSERT(!empty()); 
    auto event = events.front().second;
    events.pop();

    {
        std::stringstream ss;
        ss << *event;
        //TRACE(net) << "Emit event: " << ss.str();
    }

    hub.emitBase(event);
    delete event;
}

void EventQueue::popAndWrite(BitStreamWriter& stream) {
    ASSERT(!empty());
    auto event = events.front().second;
    events.pop();

    write(stream, event->getType().id());
    event->write(stream);
    delete event;
}

void EventQueue::add(Tick tick, EventBase* event) {
    ASSERT(empty() || tick >= events.back().first);
    events.emplace(tick, event);
}

} // namespace game
