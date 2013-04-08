#include "graphics/RenderMapObjectComponent.hpp"

#include "world/Map.hpp"
#include "opengl/ProgramManager.hpp"
#include "opengl/Texture.hpp"
#include "graphics/RenderSystem.hpp"

namespace game {

RenderMapObjectComponent::RenderMapObjectComponent(
        MapObjectComponent const* mapObject,
        TextureManager& textures)
    : mapObject(mapObject), obj(mapObject->type->modelFilename, textures) {
}

void RenderMapObjectComponent::render() {
    auto system = getManager()->system<RenderSystem>();
    auto program = system->getPrograms().load("shaders/map_vertex.glsl",
                                              "shaders/map_fragment.glsl");

    program->bind();
    program->setUniform(program->getUniformLocation("projection"),
                        system->getProjection());
    program->setUniform(program->getUniformLocation("view"),
                        system->getView());
    program->setUniform(program->getUniformLocation("model"),
                        mapObject->modelTransform);

    system->getVisionTexture().bind();
    program->setUniform(program->getUniformLocation("vision"), 0);

    for (auto& part : obj.parts) {
        program->setAttrib(0, *part.vertices);
        program->setAttrib(1, *part.normals);
        program->setUniform(program->getUniformLocation("diffuse"),
                            vec3(1, 1, 1));

        glDrawArrays(GL_TRIANGLES, 0, part.vertices->getNumElements());
    }

    program->unsetAttrib(0);
    program->unsetAttrib(1);
    program->unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace game
