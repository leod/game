#include "world/PlayerInputComponent.hpp"

#include "math/Math.hpp"
#include "physics/PhysicsComponent.hpp"
#include "world/PlayerInput.hpp"
#include "world/PlayerInputSource.hpp"

namespace game {

PlayerInputComponent::PlayerInputComponent(PhysicsComponent* physics)
    : physics(physics) {
}

void PlayerInputComponent::onPlayerInput(PlayerInput const& input) {
    runPlayerInput(physics, input);
}

} // namespace game
