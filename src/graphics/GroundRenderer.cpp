#include "graphics/GroundRenderer.hpp"

#include <GL/glew.h>

#include "opengl/ProgramManager.hpp"
#include "opengl/TextureManager.hpp"
#include "opengl/Program.hpp"
#include "world/Map.hpp"
#include "graphics/VisionSystem.hpp"

namespace game {

GroundRenderer::GroundRenderer(Map const& map,
                               TextureManager& textures,
                               ProgramManager& programs,
                               VisionSystem& visionSystem)
    : map(map),
      floorTexture(textures.load("textures/checker.png")),
      program(programs.load("shaders/ground_vertex.glsl",
                            "shaders/ground_fragment.glsl")),
      visionSystem(visionSystem),
      quadVertices(GL_ARRAY_BUFFER,
        { vec3(-0.5, -0.5, -0.5), 
          vec3( 0.5, -0.5, -0.5),
          vec3( 0.5, -0.5,  0.5),
          vec3(-0.5, -0.5,  0.5) }),
      quadNormals(GL_ARRAY_BUFFER,
        { vec3(0, 0, -1),
          vec3(0, 0, -1),
          vec3(0, 0, -1),
          vec3(0, 0, -1) }),
      quadTexCoords(GL_ARRAY_BUFFER,
        { vec2(0, 0),
          vec2(20, 0),
          vec2(20, 20),
          vec2(0, 20) }) {
}

void GroundRenderer::render(mat4 const& projection, mat4 const& view) {
    program->bind();
    program->setUniform(program->getUniformLocation("projection"),
                        projection);
    program->setUniform(program->getUniformLocation("view"),
                        view);

    visionSystem.getTexture().bind(0);
    program->setUniform(program->getUniformLocation("vision"), 0);

    floorTexture->bind(1);
    program->setUniform(program->getUniformLocation("floor"), 1);

    program->setAttrib(0, quadVertices);
    program->setAttrib(1, quadNormals);
    program->setAttrib(2, quadTexCoords);

    {
        mat4 model = glm::translate(mat4(), vec3(0, 0, 0));
        model = glm::scale(model, vec3(100, 0, 100));
        program->setUniform(program->getUniformLocation("model"),
                            model);
    }
    glDrawArrays(GL_QUADS, 0, 4);

    program->unsetAttrib(0);
    program->unsetAttrib(1);
    program->unsetAttrib(2);
    program->unbind();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, 1);
}

} // namespace game
