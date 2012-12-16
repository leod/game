#pragma once

namespace game {

struct PhysicsState {
    Vector3f position;

    PhysicsState(Vector3f position)
        : position(position) {
    }
};

} // namespace game
