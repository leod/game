#include "world/PlayerInput.hpp"

#include "physics/PhysicsComponent.hpp"

namespace game {

void runPlayerInput(PhysicsComponent* physics, PlayerInput const& input) {
    PhysicsState state = physics->getState();

    state.orientation.x = input.orientation.x;
    state.orientation.z = input.orientation.y;
    state.orientation.y = 0;

    if (input.walkForward)
        state.position += 0.3f * state.orientation;
    else if (input.walkBackward)
        state.position -= 0.3f * state.orientation;
    if (input.strafeLeft)
        state.position -= 0.3f * glm::cross(state.orientation, vec3(0, 1, 0));
    if (input.strafeRight)
        state.position += 0.3f * glm::cross(state.orientation, vec3(0, 1, 0));

    physics->setState(state);
}
 
} // namespace game
