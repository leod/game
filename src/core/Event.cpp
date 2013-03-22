#include "core/Event.hpp"

#include <sstream>

#include "core/Error.hpp"
#include "core/Log.hpp"

namespace game {

EventBase::~EventBase() {

}

EventType::EventType(size_t size, Create create, Init init, Name name)
    : size(size), create(create), init(init),  name(name) {
}

EventTypeId EventType::id() const {
    return id_;
}

void EventType::initialize(std::vector<EventType*> const& types) {
    EventType::types = types;

    // Assign ids
    EventTypeId id = 1;
    for (auto type : types)
        type->id_ = id++;

    // Log
    {
        std::stringstream ss;

        for (auto type : types)
            ss << (int)type->id_ << ":" << type->name() << ", ";
        
        INFO(net) << "Event types: "
                  << ss.str().substr(0, ss.str().size() - 2);
    }

    initialized = true;
}

EventType const& EventType::getById(EventTypeId id) {
    ASSERT(id > 0);
    ASSERT(id <= types.size());
    ASSERT(types[id - 1]->id() == id);

    return *types.at(id - 1);
}

bool EventType::initialized = false;
std::vector<EventType*> EventType::types;

std::ostream& operator<<(std::ostream& os, EventBase const& event) {
    event.print(os);
    return os;
}

} // namespace game
