#pragma once

#include "math/Math.hpp"
#include "net/Message.hpp"
#include "world/PlayerInput.hpp"

namespace game {

DEFINE_MESSAGE(PlayerInputMessage, PlayerInput);
DEFINE_MESSAGE(PlayerPositionMessage, vec3);

} // namespace game
