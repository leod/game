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

void RenderSystem::render() {
    iterate([&] (RenderComponent* component) {
        component->render();
    });
}

void RenderSystem::setCamera(vec3 position, vec3 target) {
    cameraPosition = position;
    cameraTarget = target;
}

mat4 RenderSystem::getProjection() const {
    auto projection = glm::perspective(45.0f,
            (float)window.getSize().x / (float)window.getSize().y,
            0.1f, 1000.0f);
    return projection;
}

mat4 RenderSystem::getView() const {
    auto camera = glm::lookAt(cameraPosition, cameraTarget, vec3(0, -1, 0));
    //return glm::translate(mat4(), -cameraPosition);
    return camera;
}

} // namespace game
