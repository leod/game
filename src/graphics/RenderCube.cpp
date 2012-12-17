#include "graphics/RenderCube.hpp"

namespace game {

RenderCube::RenderCube(PhysicsComponent* physics)
    : physics(physics) {
}

void RenderCube::render() {
    auto position = physics->getPosition();
    auto orientation = physics->getOrientation();

    // TODO: Draw cube using OpenGL
}

} // namespace game
