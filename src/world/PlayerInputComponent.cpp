#include "world/PlayerInputComponent.hpp"

#include <functional>

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

void PlayerInputComponent::onPlayerInput(const PlayerInput& input) {
    // TODO: This is of course just for testing.
    PhysicsState state = physics->getState();

    state.orientation.x = input.orientation.x;
    state.orientation.z = input.orientation.y;
    state.orientation.y = 0;

    if (input.walkForward)
        state.position += state.orientation;
    else if (input.walkBackward)
        state.position -= state.orientation;

    physics->setState(state);
}

} // namespace game
