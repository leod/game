#include "graphics/RenderSystem.hpp"

#include <GL/gl.h>

namespace game {

RenderSystem::RenderSystem(sf::Window const& window,
                           TextureManager& textures,
                           ProgramManager& programs)
    : window(window), textures(textures), programs(programs) {
}

TextureManager& RenderSystem::getTextures() {
    return textures;
}

ProgramManager& RenderSystem::getPrograms() {
    return programs;
}

void RenderSystem::render(ComponentItT<RenderComponent> begin,
                          ComponentItT<RenderComponent> end) {
    for (auto it = begin; it != end; ++it) {
        (*it)->render();
    }
}

void RenderSystem::setCamera(vec3 position, vec3 target) {
    cameraPosition = position;
    cameraTarget = target;
}

mat4 RenderSystem::getProjection() const {
    auto projection = glm::perspectiveFov(90.0f,
            (float)window.getSize().x,
            (float)window.getSize().y,
            1.0f, 1000.0f);
    return projection;
}

mat4 RenderSystem::getView() const {
    auto camera = glm::lookAt(cameraPosition, cameraTarget, vec3(0, 1, 0));
    return camera;
}

} // namespace game
