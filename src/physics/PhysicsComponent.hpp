#pragma once

#include "core/Component.hpp"
#include "physics/PhysicsState.hpp"

namespace game {

struct PhysicsComponent : public ComponentFamily<PhysicsComponent> {
    PhysicsComponent(vec3 const& position)
        : state(position) {
    }

    vec3 const& getPosition() const {
        return state.position;
    }
    
    vec3 const& getOrientation() const {
        return state.orientation;
    }

    void setPosition(vec3 const& position) {
        state.position = position;
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
