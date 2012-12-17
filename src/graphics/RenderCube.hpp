#pragma once

#include "math/Vector3.hpp"
#include "graphics/RenderComponent.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

struct RenderCube : public RenderComponent {
    RenderCube(PhysicsComponent*);

    // Implements RenderComponent::render
    void render();

private:
    PhysicsComponent* physics;
};

} // namespace game
