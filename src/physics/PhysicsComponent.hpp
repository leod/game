#pragma once

#include "physics/PhysicsState.hpp"

namespace game {

struct PhysicsComponent : public ComponentFamily<PhysicsComponent> {
    PhysicsComponent(const vec3& position)
        : state(position) {
    }

    const vec3& getPosition() const {
        return state.position;
    }
    
    const vec3& getOrientation() const {
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
