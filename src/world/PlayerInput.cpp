#include "world/PlayerInput.hpp"

#include "util/Print.hpp"
#include "world/PhysicsComponent.hpp"
#include "world/PhysicsSystem.hpp"

namespace game {

PlayerInput::PlayerInput()
    : strafeLeft(),
      strafeRight(),
      walkForward(),
      walkBackward(),
      shoot() {
}

PlayerInput::PlayerInput(bool strafeLeft, bool strafeRight,
                         bool walkForward, bool walkBackward,
                         bool shoot, vec2 orientation)
    : strafeLeft(strafeLeft),
      strafeRight(strafeRight),
      walkForward(walkForward),
      walkBackward(walkBackward),
      shoot(shoot),
      orientation(orientation) {
}

std::ostream& operator<<(std::ostream& os, PlayerInput const& input) {
    os << "PlayerInput("
       << input.strafeLeft
       << ", " << input.strafeRight
       << ", " << input.walkForward
       << ", " << input.walkBackward
       << ", " << input.shoot
       << ", " << input.orientation
       << ")";
    return os;
}

void runPlayerInput(PhysicsComponent* physics, PlayerInput const& input) {
    auto system = physics->getManager()->system<PhysicsSystem>();

    vec3 delta = vec3(0, 0, 0);
    vec3 orientation = vec3(input.orientation.x, 0, input.orientation.y);

    if (input.walkForward)
        delta += 0.2f * orientation;
    else if (input.walkBackward)
        delta -= 0.2f * orientation;
    if (input.strafeRight)
        delta += 0.135f * glm::cross(orientation, vec3(0, 1, 0));
    else if (input.strafeLeft)
        delta -= 0.135f * glm::cross(orientation, vec3(0, 1, 0));

    if (glm::length(delta) > 0.2f)
        delta *= (0.2f / glm::length(delta));

    system->moveOne(physics, delta);

    PhysicsState state = physics->getState();
    state.orientation = orientation;
    physics->setState(state);
}
 
} // namespace game
