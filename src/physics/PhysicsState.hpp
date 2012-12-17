#pragma once

namespace game {

struct PhysicsState {
    Vector3f position;
    Vector3f orientation; // Will probably be a quaternion

    PhysicsState(Vector3f position)
        : position(position), orientation() {
    }
};

} // namespace game
