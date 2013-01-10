#include "graphics/VisionSystem.hpp"

#include <iostream>

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

#define NUM_SAMPLES 1
        float distances[NUM_SAMPLES]; 

        for (int i = 0; i < NUM_SAMPLES; ++i) {
            float angle = 
                    glm::radians(static_cast<float>(i) / NUM_SAMPLES * 360);
            float x = glm::cos(angle);
            float z = glm::sin(angle);
            Ray ray = { position, vec3(x, 0, z) };

            Intersection intersection = rayMapIntersection(ray, map);
            distances[i] = intersection ? glm::max(intersection.get().first,
                                                   300.0f)
                                        : 300;
            std::cout << distances[i] << std::endl;
        }
    });
}

} // namespace game
