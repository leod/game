#include "graphics/RenderSystem.hpp"

#include <GL/gl.h>

namespace game {

RenderSystem::RenderSystem(sf::Window* window)
    : window(window) {
}

void RenderSystem::render(ComponentItT<RenderComponent> begin,
                          ComponentItT<RenderComponent> end) {
    auto projection = glm::perspectiveFov(90.0f,
            //(float)window->getSize().x / window->getSize().y,
            (float)window->getSize().x,
            (float)window->getSize().y,
            1.0f, 1000.0f);
    auto camera = glm::lookAt(cameraPosition, cameraTarget, vec3(0, 1, 0));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(camera));

    for (auto it = begin; it != end; ++it) {
        (*it)->render();
    }
}

void RenderSystem::setCamera(vec3 position, vec3 target) {
    cameraPosition = position;
    cameraTarget = target;
}

} // namespace game
