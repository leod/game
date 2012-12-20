#pragma once

#include <SFML/Window.hpp>

#include "core/EntityRegistry.hpp"
#include "core/System.hpp"
#include "math/Math.hpp"
#include "graphics/RenderComponent.hpp"

namespace game {

struct RenderSystem : public SystemBase<RenderComponent> {
    RenderSystem(sf::Window* window);

    void render(ComponentItT<RenderComponent> begin,
                ComponentItT<RenderComponent> end);

    void setCamera(vec3 position, vec3 target);

private:
    sf::Window* window;

    vec3 cameraPosition, cameraTarget;
};

} // namespace game
