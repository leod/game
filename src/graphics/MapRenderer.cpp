#include "graphics/MapRenderer.hpp"

#include <GL/glew.h>

#include "opengl/ProgramManager.hpp"
#include "opengl/TextureManager.hpp"
#include "opengl/Program.hpp"
#include "world/Map.hpp"
#include "graphics/VisionSystem.hpp"

namespace game {

MapRenderer::MapRenderer(Map const& map,
                         TextureManager& textures,
                         ProgramManager& programs,
                         VisionSystem& vision)
    : map(map),
      program(programs.load("shaders/map_vertex.glsl",
                            "shaders/map_fragment.glsl")),
      vision(vision),
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
          vec3(0, 0, 1) }),
      floor(textures.load("textures/checker.png")) {
}

void MapRenderer::render(mat4 const& projection, mat4 const& view) {
    /*floor->bind(); {
        float t = 5.0f;

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-35, 0, -35);

        glTexCoord2f(t, 0);
        glVertex3f(35, 0, -35);

        glTexCoord2f(t, t);
        glVertex3f(35, 0, 35);

        glTexCoord2f(0, t);
        glVertex3f(-35, 0, 35);

        glEnd();
    }*/

    program->bind();

    program->setUniform(program->getUniformLocation("projection"),
                        projection);
    program->setUniform(program->getUniformLocation("view"),
                        view);
    program->setUniform(program->getUniformLocation("diffuse"),
                        vec3(1, 0, 0));
    vision.getTexture().bind();
    program->setUniform(program->getUniformLocation("vision"), 0);

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

    // Floor hack
    if(1){
        program->setUniform(program->getUniformLocation("diffuse"),
                            vec3(0.7, 0.7, 0.7));
        mat4 model = glm::translate(mat4(), vec3(0, 0, 0));
        model = glm::scale(model, vec3(100, 0, 100));
        program->setUniform(program->getUniformLocation("model"),
                            model);
        glDrawArrays(GL_QUADS, 16, 4);
    }

    program->unsetAttrib(0);
    program->unsetAttrib(1);
    program->unbind();

    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace game
