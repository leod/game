#include "net/MessageTypes.hpp"

#include <vector>

#include "net/MessageHub.hpp"
#include "world/MessageTypes.hpp"

namespace game {

static std::vector<MessageType*> messageTypes = {
    &CreateEntityMessage::type,
    &RemoveEntityMessage::type,
    &LoggedInMessage::type,
    &DisconnectMessage::type,
    &ClientConnectedMessage::type,
    &ClientDisconnectedMessage::type,

    &PlayerInputMessage::type,
    &PlayerPositionMessage::type
};

void initializeMessageTypes() {
    MessageType::initialize(messageTypes);
}

} // namespace game
