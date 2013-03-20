#pragma once

#include "math/Math.hpp"
#include "opengl/Buffer.hpp"
#include "graphics/RenderComponent.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

struct RenderCube : public RenderComponent {
    RenderCube(PhysicsComponent const*, vec3 color, float scale = 1.0f);

    // Implements RenderComponent::render
    void render();

private:
    PhysicsComponent const* physics;
    vec3 color;
    float scale;

    Buffer<vec3> positions;
    Buffer<vec3> normals;
    Buffer<GLshort> indices;
};

} // namespace game
