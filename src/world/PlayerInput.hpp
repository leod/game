#pragma once

#include "math/Math.hpp"

namespace game {

struct PhysicsComponent;

struct PlayerInput {
    bool strafeLeft, strafeRight,
         walkForward, walkBackward;
    vec2 orientation;

    PlayerInput()
        : strafeLeft(),
          strafeRight(),
          walkForward(),
          walkBackward() {
    }
};

void runPlayerInput(PhysicsComponent*, PlayerInput const&);

} // namespace game
