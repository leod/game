#include "graphics/VisionSystem.hpp"

#include <iostream>

#include <GL/glew.h>

#include "util/Print.hpp"
#include "map/Map.hpp"
#include "opengl/ProgramManager.hpp"
#include "opengl/Program.hpp"

namespace game {

VisionSystem::VisionSystem(Map const& map, ProgramManager& programs)
    : map(map), program(programs.load("shaders/vision_vertex.glsl",
                                      "shaders/vision_fragment.glsl")) {
}

void VisionSystem::renderVision(mat4 const& projection, mat4 const& view) {
    iterate([&] (VisionComponent* component) {
        auto position = component->getPosition();

#define NUM_SAMPLES 100
#define CUTOFF 13.0f
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

            distances[i] = intersection ? glm::min(intersection.get().first,
                                                   CUTOFF)
                                        : CUTOFF;
            //std::cout << distances[i] << std::endl;
        }

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 1, 1);
        glVertex3f(position.x, position.y, position.z);

        for (int i = 0; i < NUM_SAMPLES; ++i) {
            vec3 target = position + distances[i] * directions[i];
            glVertex3f(target.x, target.y, target.z);
        }

        vec3 target = position + distances[0] * directions[0];
        glVertex3f(target.x, target.y, target.z);

        glEnd();
    });
}

} // namespace game
