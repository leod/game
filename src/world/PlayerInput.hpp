#pragma once

#include "math/Math.hpp"

namespace game {

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

} // namespace game
