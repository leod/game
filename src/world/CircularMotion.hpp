#pragma once

#include <iostream>
#include "util/Print.hpp"

#include "world/PhysicsComponent.hpp"
#include "world/TickComponent.hpp"

namespace game {

struct CircularMotion : public TickComponent {
    CircularMotion(PhysicsComponent* physics)
        : physics(physics) {
    }

    void tick() {
        auto state = physics->getState();

        state.orientation.x = glm::sign(glm::sin((float) time / 20)) * 0.3;
        //state.orientation.x = state.orientation.x;
        //state.orientation.z = glm::cos((float) time / 10);
        //state.orientation.z = state.orientation.z;
        state.orientation.y = 0;

        state.position += 0.09f * state.orientation;

        physics->setState(state);

        time++;
    }

private:
    int time;

    PhysicsComponent* physics;
};

} // namespace game
