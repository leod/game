#include "server/PlayerInputComponent.hpp"

#include "math/Math.hpp"
#include "world/PhysicsComponent.hpp"
#include "world/PlayerInput.hpp"

namespace game {

PlayerInputComponent::PlayerInputComponent(PhysicsComponent* physics)
    : physics(physics) {
}

void PlayerInputComponent::onPlayerInput(PlayerInput const& input) {
    runPlayerInput(physics, input);
}

} // namespace game
