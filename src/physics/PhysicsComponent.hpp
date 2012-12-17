#pragma once

#include "math/Vector3.hpp"
#include "math/Vector2.hpp"

#include "physics/PhysicsState.hpp"

namespace game {

struct PhysicsComponent : public ComponentFamily<PhysicsComponent> {
    PhysicsComponent(const Vector3f& position)
        : state(position) {
    }

    const Vector3f& getPosition() const {
        return state.position;
    }
    
    const Vector3f& getOrientation() const {
        return state.orientation;
    }

    const PhysicsState& getState() const {
        return state;
    }

    void setState(const PhysicsState& newState) {
        state = newState;
    }

private:
    friend class PhysicsSystem;

    PhysicsState state;
};

} // namespace game
