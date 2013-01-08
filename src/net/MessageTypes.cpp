#include "net/MessageTypes.hpp"

#include "net/MessageHub.hpp"

#include "world/MessageTypes.hpp"

namespace game {

MessageHub* makeMessageHub() {
    return MessageHub::make<CreateEntityMessage,
                            LoginMessage,
                            PlayerInputMessage>();
}

} // namespace game
