#include "world/EventTypes.hpp"

#include <iostream>

#include "net/EventTypes.hpp"

namespace game {

static std::vector<EventType*> eventTypes = {
    &CreateEntityOrder::type,
    &RemoveEntityOrder::type,
    &LoggedInOrder::type,
    &DisconnectWish::type,
    &ClientConnectedOrder::type,
    &ClientDisconnectedOrder::type,

    &PlayerInputWish::type,
    &PlayerPositionOrder::type
};

void initializeEventTypes() {
    EventType::initialize(eventTypes);
}

}
