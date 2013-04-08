#include "graphics/VisionSystem.hpp"

#include <iostream>

#include <GL/glew.h>

#include "util/Print.hpp"
#include "world/Map.hpp"
#include "opengl/ProgramManager.hpp"
#include "opengl/Program.hpp"

#define ENABLE

namespace game {

VisionSystem::VisionSystem(Map const& map, ProgramManager& programs)
    : map(map),
      program(programs.load("shaders/vision_vertex.glsl",
                            "shaders/vision_fragment.glsl")),
      framebuffer(Framebuffer::COLOR, ivec2(1280, 1024)) {
}

void VisionSystem::renderVision(mat4 const&, mat4 const&) {
#ifdef ENABLE
    iterate([&] (VisionComponent* component) {
        auto position = component->getPosition();

#define NUM_SAMPLES 360
#define CUTOFF 15.0f
        vec3 directions[NUM_SAMPLES];
        float distances[NUM_SAMPLES]; 
        MapObjectComponent const* objects[NUM_SAMPLES];

        for (int i = 0; i < NUM_SAMPLES; ++i) {
            float angle = 
                    glm::radians(static_cast<float>(i) / NUM_SAMPLES * 360);
            float x = glm::cos(angle);
            float z = glm::sin(angle);

            directions[i] = vec3(x, 0, z);
            //std::cout << directions[i] << std::endl;

            Ray ray = { position, directions[i] };
            MapObjectComponent const* object;
            Intersection intersection =
                rayMapIntersection(ray, map, &object);

            if (intersection) {
                distances[i] = glm::min(intersection.get(), CUTOFF);
                objects[i] = object;
                
                //std::cout << "HIT " << directions[i] << std::endl;
            } else {
                distances[i] = CUTOFF;
                objects[i] = nullptr;
            }

            //std::cout << distances[i] << std::endl;
        }

        glClearColor(0.0, 0.0, 0.0, 1.0);
        framebuffer.renderInto(
        [&position, &directions, &distances, &objects] () {
            glBegin(GL_TRIANGLE_FAN);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(position.x, position.y, position.z);

            for (int i = 0; i < NUM_SAMPLES; ++i) {
                vec3 target = position + distances[i] * directions[i];
                float targetY = objects[i] ? objects[i]->bbox.max.y : target.y;
                glVertex3f(target.x, targetY, target.z);
            }

            vec3 target = position + distances[0] * directions[0];
            float targetY = objects[0] ? objects[0]->bbox.max.y : target.y;
            glVertex3f(target.x, targetY, target.z);

            glEnd();
        }, Framebuffer::CLEAR);
    });
#else
    glClearColor(1.0, 1.0, 1.0, 1.0);
    framebuffer.renderInto([] () {}, Framebuffer::CLEAR);
#endif
}

Texture const& VisionSystem::getTexture() const {
    return framebuffer.getColorTexture();
}

} // namespace game
