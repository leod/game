#pragma once

#include "net/Message.hpp"
#include "world/PlayerInput.hpp"

namespace game {

struct PlayerInputMessage
    : public Message<PlayerInputMessage,
                     PlayerInput>
{};

} // namespace game
