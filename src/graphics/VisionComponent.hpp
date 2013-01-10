#pragma once

#include "core/Component.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

struct VisionComponent : ComponentFamily<VisionComponent> {
    VisionComponent(PhysicsComponent const* physics)
        : physics(physics) {
    }

    vec3 const& getPosition() const {
        return physics->getPosition();
    }

private:
    PhysicsComponent const* const physics;
};

} // namespace game
