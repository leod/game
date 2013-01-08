#include "world/PlayerInputComponent.hpp"

#include <functional>

#include "math/Math.hpp"
#include "physics/PhysicsComponent.hpp"
#include "world/PlayerInput.hpp"
#include "world/PlayerInputSource.hpp"

namespace game {

PlayerInputComponent::PlayerInputComponent( PhysicsComponent* physics)
    : physics(physics) {
}

void PlayerInputComponent::tick() {
    runPlayerInput(physics, playerInput);
}

void PlayerInputComponent::onPlayerInput(PlayerInput const& input) {
    playerInput = input;
}

} // namespace game
