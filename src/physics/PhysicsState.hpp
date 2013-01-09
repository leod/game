#pragma once

#include "math/Math.hpp"

namespace game {

struct PhysicsState {
    vec3 position;
    vec3 orientation; // Will probably be a quaternion

    PhysicsState(vec3 const& position)
        : position(position), orientation(0, 0, 0) {
    }
};

} // namespace game
