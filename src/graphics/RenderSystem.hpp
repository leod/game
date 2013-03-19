#pragma once

#include <SFML/Window.hpp>

#include "core/EntityManager.hpp"
#include "core/System.hpp"
#include "math/Math.hpp"
#include "graphics/RenderComponent.hpp"

namespace game {

struct TextureManager;
struct ProgramManager;
struct VisionSystem;
struct Texture;

struct RenderSystem : public SystemBase<RenderComponent> {
    RenderSystem(sf::Window const& window,
                 TextureManager& textures,
                 ProgramManager& programs,
                 VisionSystem& vision);

    TextureManager& getTextures();
    ProgramManager& getPrograms();

    void render();

    // These definitely don't belong here
    void setCamera(vec3 position, vec3 target);

    mat4 getProjection() const;
    mat4 getView() const;

    Texture const& getVisionTexture() const;

private:
    sf::Window const& window;
    TextureManager& textures;
    ProgramManager& programs;
    VisionSystem& vision;

    vec3 cameraPosition, cameraTarget;
};

} // namespace game
