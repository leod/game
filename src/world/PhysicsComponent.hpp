#pragma once

#include "core/Component.hpp"
#include "world/PhysicsState.hpp"

namespace game {

struct PhysicsComponent : public ComponentFamily<PhysicsComponent> {
    PhysicsComponent()
        : state(vec3(0, 0, 0)) {
    }

    PhysicsComponent(vec3 const& position)
        : state(position) {
    }

    PhysicsComponent(vec3 const& position, vec3 const& orientation)
        : state(position, orientation) {
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

    void setOrientation(vec3 const& orientation) {
        state.orientation = orientation;
    }

    PhysicsState const& getState() const {
        return state;
    }

    void setState(PhysicsState const& newState) {
        state = newState;
    }

private:
    friend struct PhysicsSystem;

    PhysicsState state;

    float radius; // tmp
};

} // namespace game
