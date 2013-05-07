#pragma once

#include "math/Math.hpp"
#include "net/Event.hpp"
#include "world/PlayerInput.hpp"

namespace game {

DEFINE_WISH(PlayerInputWish, PlayerInput);
DEFINE_ORDER(PlayerPositionOrder, vec3);

DEFINE_ORDER(PlayerKilledOrder, ClientId, ClientId);

void initializeEventTypes();

} // namespace game
