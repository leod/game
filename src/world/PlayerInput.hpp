#pragma once

#include <ostream>

#include "math/Math.hpp"

namespace game {

struct PhysicsComponent;

struct PlayerInput {
    bool strafeLeft, strafeRight,
         walkForward, walkBackward;
    bool shoot;
    vec2 orientation;

    PlayerInput();
    PlayerInput(bool strafeLeft, bool strafeRight,
                bool walkForward, bool walkBackward,
                bool shoot, vec2 orientation);

    bool isWalking() const;
};

std::ostream& operator<<(std::ostream&, PlayerInput const&);

void runPlayerInput(PhysicsComponent*, PlayerInput const&);

} // namespace game
