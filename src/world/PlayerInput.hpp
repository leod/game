#pragma once

#include "math/Vector2.hpp"

namespace game {

struct PlayerInput {
    bool strafeLeft, strafeRight,
         walkForward, walkBackward;
    Vector2f orientation;

    PlayerInput()
        : strafeLeft(),
          strafeRight(),
          walkForward(),
          walkBackward() {
    }
};

} // namespace game
