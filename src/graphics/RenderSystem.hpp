#pragma once

#include <SFML/Window.hpp>

#include "core/EntityRegistry.hpp"
#include "core/System.hpp"
#include "math/Math.hpp"
#include "graphics/RenderComponent.hpp"

namespace game {

struct TextureManager;
struct ProgramManager;

struct RenderSystem : public SystemBase<RenderComponent> {
    RenderSystem(sf::Window const& window,
                 TextureManager& textures,
                 ProgramManager& programs);

    TextureManager& getTextures();
    ProgramManager& getPrograms();

    void render(ComponentItT<RenderComponent> begin,
                ComponentItT<RenderComponent> end);

    void setCamera(vec3 position, vec3 target);

private:
    sf::Window const& window;
    TextureManager& textures;
    ProgramManager& programs;

    vec3 cameraPosition, cameraTarget;
};

} // namespace game
