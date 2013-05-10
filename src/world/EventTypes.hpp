#pragma once

#include "math/Math.hpp"
#include "net/Event.hpp"
#include "world/PlayerInput.hpp"

namespace game {

// Local events
DEFINE_LOCAL_EVENT(PlayerHurtLocal, ClientId, int);

// Network events
DEFINE_WISH(PlayerInputWish, PlayerInput);
DEFINE_ORDER(PlayerPositionOrder, vec3);
DEFINE_ORDER(PlayerKilledOrder, ClientId, ClientId);

void initializeEventTypes();

} // namespace game
