#pragma once

#include "graphics/OBJ.hpp"
#include "graphics/RenderComponent.hpp"

namespace game {

struct PhysicsComponent;
struct TextureManager;

struct RenderPlayerComponent : public RenderComponent {
    RenderPlayerComponent(PhysicsComponent const*, vec3 const& color,
                          TextureManager&);

    // Implement RenderComponent
    void render() override;

private:
    PhysicsComponent const* physics;
    vec3 color;

    OBJ obj;
};

} // namespace game
