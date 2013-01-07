#include "net/MessageTypes.hpp"

#include "net/MessageHub.hpp"

namespace game {

MessageHub* makeMessageHub() {
    return MessageHub::make<CreateEntity>();
}

} // namespace game
