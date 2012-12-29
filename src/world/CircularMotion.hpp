#pragma once

#include "physics/PhysicsComponent.hpp"
#include "world/TickComponent.hpp"

namespace game {

struct CircularMotion : public TickComponent {
    CircularMotion(PhysicsComponent* physics)
        : physics(physics) {
    }

    void tick() {
        auto state = physics->getState();

        state.orientation.x = glm::sin((float) time / 90);
        state.orientation.x = state.orientation.x;
        state.orientation.z = glm::cos((float) time / 90);
        state.orientation.z = state.orientation.z;
        state.orientation.y = 0;

        state.position += 0.02f * state.orientation;

        physics->setState(state);

        time++;
    }

private:
    int time;

    PhysicsComponent* physics;
};

} // namespace game
