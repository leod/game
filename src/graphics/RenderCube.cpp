#include "graphics/RenderCube.hpp"

#include <GL/glew.h>

#include "core/EntityRegistry.hpp"
#include "opengl/ProgramManager.hpp"
#include "graphics/RenderSystem.hpp"

namespace game {

RenderCube::RenderCube(PhysicsComponent const* physics, vec3 color)
    : physics(physics), color(color),
      positions(GL_ARRAY_BUFFER, std::vector<vec3> {
          vec3(-0.5, -0.5,  0.5),
          vec3( 0.5, -0.5,  0.5),
          vec3(-0.5,  0.5,  0.5),
          vec3(-0.5,  0.5,  0.5),
          vec3(-0.5, -0.5, -0.5),
          vec3( 0.5, -0.5, -0.5),
          vec3( 0.5,  0.5, -0.5),
          vec3(-0.5,  0.5, -0.5) }),
      indices(GL_ELEMENT_ARRAY_BUFFER, std::vector<GLushort> {
          0, 1, 2,
          2, 3, 0,

          1, 5, 6,
          6, 2, 1,

          7, 6, 5,
          5, 4, 7,

          4, 0, 3,
          3, 7, 4,

          4, 5, 1,
          1, 0, 4,

          3, 2, 6,
          6, 7, 3 }) {
}

void RenderCube::render() {
    auto system = getRegistry()->system<RenderSystem>();
    auto program = system->getPrograms().load("shaders/cube_vertex.glsl",
                                              "shaders/cube_fragment.glsl");

    auto modelview = mat4();
    {
        auto position = physics->getPosition();
        auto orientation = physics->getOrientation();
        auto angle = glm::atan(orientation.x, orientation.z);

        modelview = glm::translate(modelview, position);
        modelview = glm::rotate(modelview, glm::degrees(angle), vec3(0, 1, 0));
    }

    program->bind();
    program->setUniform(program->getUniformLocation("projection"),
                        system->getProjection());
    program->setUniform(program->getUniformLocation("modelview"),
                        system->getModelview() * modelview);
    program->setAttrib(program->getAttribLocation("position"),
                        positions);

    drawElements(indices, GL_TRIANGLES);

    program->unsetAttrib(program->getAttribLocation("position"));
}

} // namespace game
