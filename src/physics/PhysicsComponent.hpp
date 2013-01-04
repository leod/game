#pragma once

#include "core/Component.hpp"
#include "physics/PhysicsState.hpp"

namespace game {

struct PhysicsComponent : public ComponentFamily<PhysicsComponent> {
    PhysicsComponent(const vec3& position)
        : state(position) {
    }

    vec3 const& getPosition() const {
        return state.position;
    }
    
    vec3 const& getOrientation() const {
        return state.orientation;
    }

    PhysicsState const& getState() const {
        return state;
    }

    void setState(PhysicsState const& newState) {
        state = newState;
    }

private:
    friend class PhysicsSystem;

    PhysicsState state;
};

} // namespace game
