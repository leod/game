#include "world/LocalPlayerInputComponent.hpp"

#include "core/Log.hpp"
#include "math/Math.hpp"
#include "world/PhysicsComponent.hpp"
#include "world/PlayerInput.hpp"
#include "world/PlayerInputSource.hpp"

namespace game {

LocalPlayerInputComponent::LocalPlayerInputComponent(PhysicsComponent* physics)
    : physics(physics) {
}

void LocalPlayerInputComponent::onPlayerInput(PlayerInput const& input) {
    runPlayerInput(physics, input);

    log.push_back({ input, physics->getPosition() });
}

void LocalPlayerInputComponent::onCorrection(vec3 const& position) {
    if (log.empty()) {
        WARN(world) << "Received correction but log is empty";
        return;
    }

    auto predicted = log.front();
    log.pop_front();

    const float threshold = 0.1;
    if (glm::length(position - predicted.position) > threshold) {
        TRACE(world) << "Received correction: " << position << " vs our "
                     << predicted.position << ". Replaying";

        replay(position);
    }
}

void LocalPlayerInputComponent::replay(vec3 const& start) {
    physics->setPosition(start);

    for (auto const& entry : log)
        runPlayerInput(physics, entry.input);
}

} // namespace game
