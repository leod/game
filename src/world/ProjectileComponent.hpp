#pragma once

#include "core/Component.hpp"

namespace game {
   
struct PhysicsComponent;

struct ProjectileComponent : ComponentFamily<ProjectileComponent> {
    enum : bool {
        GLOBAL = false,
        LOCAL = true
    };

    ProjectileComponent(PhysicsComponent* physics, bool isLocal_)
        : physics(physics), isLocal_(isLocal_), lifetime(0) {
    }

    bool isLocal() {
        return isLocal_;
    }

    PhysicsComponent* getPhysics() {
        return physics;
    }

private:
    PhysicsComponent* physics;
    bool isLocal_;

    int lifetime;

    friend struct ProjectileSystem;
};

} // namespace game
