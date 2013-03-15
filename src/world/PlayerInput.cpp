#include "world/PlayerInput.hpp"

#include "util/Print.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

PlayerInput::PlayerInput()
    : strafeLeft(),
      strafeRight(),
      walkForward(),
      walkBackward() {
}

PlayerInput::PlayerInput(bool strafeLeft, bool strafeRight,
                         bool walkForward, bool walkBackward,
                         vec2 orientation)
    : strafeLeft(strafeLeft),
      strafeRight(strafeRight),
      walkForward(walkForward),
      walkBackward(walkBackward),
      orientation(orientation) {
}

std::ostream& operator<<(std::ostream& os, PlayerInput const& input) {
    os << "PlayerInput("
       << input.strafeLeft
       << ", " << input.strafeRight
       << ", " << input.walkForward
       << ", " << input.walkBackward
       << ", " << input.orientation
       << ")";
    return os;
}

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
