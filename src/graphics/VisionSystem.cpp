#include "graphics/VisionSystem.hpp"

#include <iostream>

#include <GL/glew.h>

#include "util/Print.hpp"
#include "map/Map.hpp"
#include "opengl/ProgramManager.hpp"
#include "opengl/Program.hpp"

namespace game {

VisionSystem::VisionSystem(Map const& map, ProgramManager& programs)
    : map(map),
      program(programs.load("shaders/vision_vertex.glsl",
                            "shaders/vision_fragment.glsl")),
      framebuffer(Framebuffer::COLOR, ivec2(512, 512)) {
}

void VisionSystem::renderVision(mat4 const& projection, mat4 const& view) {
    iterate([&] (VisionComponent* component) {
        auto position = component->getPosition();

#define NUM_SAMPLES 150
#define CUTOFF 30.0f
        vec3 directions[NUM_SAMPLES];
        float distances[NUM_SAMPLES]; 

        for (int i = 0; i < NUM_SAMPLES; ++i) {
            float angle = 
                    glm::radians(static_cast<float>(i) / NUM_SAMPLES * 360);
            float x = glm::cos(angle);
            float z = glm::sin(angle);
            directions[i] = vec3(x, 0, z);
            //std::cout << directions[i] << std::endl;

            Ray ray = { position, directions[i] };
            Intersection intersection = rayMapIntersection(ray, map);

            distances[i] = (
                intersection ? glm::min(intersection.get().first, CUTOFF)
                             : CUTOFF);
            //std::cout << distances[i] << std::endl;
        }

        glClearColor(0.0, 0.0, 0.0, 1.0);
        framebuffer.renderInto([&position, &distances, &directions] () {
            glBegin(GL_TRIANGLE_FAN);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(position.x, position.y, position.z);

            for (int i = 0; i < NUM_SAMPLES; ++i) {
                vec3 target = position + distances[i] * directions[i];
                glVertex3f(target.x, target.y, target.z);
            }

            vec3 target = position + distances[0] * directions[0];
            glVertex3f(target.x, target.y, target.z);

            glEnd();
        }, Framebuffer::CLEAR);
    });
}

Texture const& VisionSystem::getTexture() const {
    return framebuffer.getColorTexture();
}

} // namespace game
