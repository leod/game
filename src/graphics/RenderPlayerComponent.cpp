#include "graphics/RenderPlayerComponent.hpp"

#include "core/Log.hpp"
#include "opengl/ProgramManager.hpp"
#include "opengl/Texture.hpp"
#include "graphics/RenderSystem.hpp"
#include "world/PhysicsComponent.hpp"

namespace game {

RenderPlayerComponent::RenderPlayerComponent(PhysicsComponent const* physics,
                                             vec3 const& color, 
                                             TextureManager& textures)
    : physics(physics), color(color), obj("soldier/soldier1.obj", textures) {
}

void RenderPlayerComponent::render() {
    auto system = getManager()->system<RenderSystem>();
    auto program = system->getPrograms().load("shaders/object_vertex.glsl",
                                              "shaders/object_fragment.glsl");

    auto model = mat4();
    {
        auto position = physics->getPosition();
        auto orientation = physics->getOrientation();
        auto angle = glm::atan(orientation.x, orientation.z);

        model = glm::translate(model, position);
        model = glm::rotate(model, glm::degrees(angle) - 188, vec3(0, 1, 0));
        model = glm::scale(model, vec3(0.3, 0.3, 0.3));
    }

    program->bind();
    program->setUniform(program->getUniformLocation("projection"),
                        system->getProjection());
    program->setUniform(program->getUniformLocation("view"),
                        system->getView());
    program->setUniform(program->getUniformLocation("model"),
                        model);

    system->getVisionTexture().bind();
    program->setUniform(program->getUniformLocation("vision"), 0);

    size_t i = 0;
    for (auto& part : obj.parts) {
        program->setAttrib(0, *part.vertices);
        program->setAttrib(1, *part.normals);

        vec3 diffuse = (i == 0) ? this->color : part.material.diffuse;
        program->setUniform(program->getUniformLocation("diffuse"), diffuse);

        glDrawArrays(GL_TRIANGLES, 0, part.vertices->getNumElements());

        i++;
    }

    program->unsetAttrib(0);
    program->unsetAttrib(1);
    program->unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace game
