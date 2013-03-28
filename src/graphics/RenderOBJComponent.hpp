#pragma once

#include "graphics/OBJ.hpp"
#include "graphics/RenderComponent.hpp"

namespace game {

struct PhysicsComponent;
struct TextureManager;

struct RenderOBJComponent : public RenderComponent {
    RenderOBJComponent(PhysicsComponent const*, std::string const& filename,
                       TextureManager&);

    // Implement RenderComponent
    void render() override;

private:
    PhysicsComponent const* physics;

    OBJ obj;
};

} // namespace game
