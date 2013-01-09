#include "graphics/MapRenderer.hpp"

#include <GL/glew.h>

#include "opengl/ProgramManager.hpp"
#include "opengl/Program.hpp"
#include "map/Map.hpp"

namespace game {

MapRenderer::MapRenderer(Map const& map, TextureManager&, ProgramManager& programs)
    : map(map),
      program(programs.load("shaders/cube_vertex.glsl",
                            "shaders/cube_fragment.glsl")),
      cubePositions(GL_ARRAY_BUFFER,
        { vec3(-0.5, -0.5,  0.5), // Front
          vec3( 0.5, -0.5,  0.5),
          vec3( 0.5,  0.5,  0.5),
          vec3(-0.5,  0.5,  0.5),

          vec3( 0.5, -0.5,  0.5), // Right
          vec3( 0.5, -0.5, -0.5),
          vec3( 0.5,  0.5, -0.5),
          vec3( 0.5,  0.5,  0.5),

          vec3( 0.5, -0.5, -0.5), // Back
          vec3(-0.5, -0.5, -0.5),
          vec3(-0.5,  0.5, -0.5),
          vec3( 0.5,  0.5, -0.5),

          vec3(-0.5, -0.5, -0.5), // Left
          vec3(-0.5, -0.5,  0.5),
          vec3(-0.5,  0.5,  0.5),
          vec3(-0.5,  0.5, -0.5),

          vec3(-0.5, -0.5, -0.5), // Bottom
          vec3( 0.5, -0.5, -0.5),
          vec3( 0.5, -0.5,  0.5),
          vec3(-0.5, -0.5,  0.5),

          vec3(-0.5,  0.5,  0.5), // Top
          vec3( 0.5,  0.5,  0.5),
          vec3( 0.5,  0.5, -0.5),
          vec3(-0.5,  0.5, -0.5) }),
    cubeNormals(GL_ARRAY_BUFFER,
        { vec3(0, 0, 1),
          vec3(0, 0, 1),
          vec3(0, 0, 1),
          vec3(0, 0, 1),

          vec3(1, 0, 0),
          vec3(1, 0, 0),
          vec3(1, 0, 0),
          vec3(1, 0, 0),

          vec3(0, 0, -1),
          vec3(0, 0, -1),
          vec3(0, 0, -1),
          vec3(0, 0, -1),

          vec3(-1, 0, 0),
          vec3(-1, 0, 0),
          vec3(-1, 0, 0),
          vec3(-1, 0, 0),

          vec3(0, 0, -1),
          vec3(0, 0, -1),
          vec3(0, 0, -1),
          vec3(0, 0, -1),

          vec3(0, 0, 1),
          vec3(0, 0, 1),
          vec3(0, 0, 1),
          vec3(0, 0, 1) }) {
}

void MapRenderer::render(mat4 const& projection, mat4 const& view) {
    glDisable(GL_CULL_FACE);

    program->bind();
    program->setUniform(program->getUniformLocation("projection"),
                        projection);
    program->setUniform(program->getUniformLocation("view"),
                        view);
    program->setUniform(program->getUniformLocation("diffuse"),
                        vec3(1, 0, 0));

    program->setAttrib(0, cubePositions);
    program->setAttrib(1, cubeNormals);

    for (auto& block : map.getBlocks()) {
        mat4 model = glm::translate(mat4(),
                block.groundCenter + vec3(0, block.scale.y / 2, 0));
        model = glm::rotate(model, block.yRotation, vec3(0, 1, 0));
        model = glm::scale(model, block.scale);

        program->setUniform(program->getUniformLocation("model"),
                            model);
        glDrawArrays(GL_QUADS, 0, 24);
    }

    program->unsetAttrib(0);
    program->unsetAttrib(1);
    program->unbind();
}

} // namespace game
