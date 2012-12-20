#pragma once

#include "math/Math.hpp"
#include "graphics/RenderComponent.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

struct RenderCube : public RenderComponent {
    RenderCube(PhysicsComponent const*, vec3 color);

    // Implements RenderComponent::render
    void render();

private:
    PhysicsComponent const* physics;
    vec3 color;
};

} // namespace game
