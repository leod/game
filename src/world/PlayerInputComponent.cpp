#include "world/PlayerInputComponent.hpp"

#include <functional>

#include "math/Math.hpp"
#include "physics/PhysicsComponent.hpp"
#include "world/PlayerInput.hpp"
#include "world/PlayerInputSource.hpp"

namespace game {

PlayerInputComponent::PlayerInputComponent(PlayerInputSource* source,
                                           PhysicsComponent* physics)
    : physics(physics) {
    source->onPlayerInput.connect(std::bind(
            &PlayerInputComponent::onPlayerInput,
            this, std::placeholders::_1));
}

void PlayerInputComponent::tick() {
    // TODO: This is of course just for testing.
    PhysicsState state = physics->getState();

    state.orientation.x = playerInput.orientation.x;
    state.orientation.z = playerInput.orientation.y;
    state.orientation.y = 0;

    if (playerInput.walkForward)
        state.position += 0.3f * state.orientation;
    else if (playerInput.walkBackward)
        state.position -= 0.3f * state.orientation;
    else if (playerInput.strafeLeft)
        state.position -= 0.1f * glm::cross(state.orientation, vec3(0, 1, 0));
    else if (playerInput.strafeRight)
        state.position += 0.1f * glm::cross(state.orientation, vec3(0, 1, 0));

    physics->setState(state);
}

void PlayerInputComponent::onPlayerInput(PlayerInput const& input) {
    playerInput = input;
}

} // namespace game
