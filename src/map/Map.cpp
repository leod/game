#include "map/Map.hpp"

#include <iostream>

#include <boost/none.hpp>

#include "math/Intersection.hpp"
#include "util/Print.hpp"

using boost::none;

namespace game {

Map::Block::Block(vec3 groundCenter, float yRotation, vec3 scale)
    : groundCenter(groundCenter), yRotation(yRotation), scale(scale),
      rotScale(vec3(glm::rotate(yRotation, vec3(0, 1, 0)) *
                    vec4(scale, 1.0))) {
}

Map::Map()
    : blocks({
              { vec3(-7, 0, 0), 0, vec3(1, 2, 7) },
              { vec3(7, 0, 0), 0, vec3(1, 2, 7) },
              { vec3(0, 0, -15), 0, vec3(15, 3, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(-5, 0, -8), 90, vec3(5, 1, 1) },
              { vec3(6, 0, -9), 90, vec3(5, 1, 1) }
             }) {
}

std::vector<Map::Block> const& Map::getBlocks() const {
    return blocks;
}

static Intersection
rayBlockIntersection(Ray const& ray, Map::Block const& block) {
    Intersection intersection;
    auto check = [&intersection] (Intersection const& i) {
        if (!intersection || (i && i.get().first < intersection.get().first))
            intersection = i;
    };

    auto c = block.groundCenter;
    auto s = block.rotScale;

    // Bottom quad
    check(rayQuadIntersection(ray, { c,
            vec3(s.x / 2, 0, 0), vec3(0, 0, s.z / 2) }));

    // Top quad
    check(rayQuadIntersection(ray, { c + s.y,
            vec3(s.x / 2, 0, 0), vec3(0, 0, s.z / 2) }));

    // Left quad
    check(rayQuadIntersection(ray, { c + vec3(-s.x / 2, s.y / 2, 0),
            vec3(0, s.y / 2, 0), vec3(0, 0, s.z / 2) }));

    // Right quad
    check(rayQuadIntersection(ray, { c + vec3(s.x / 2, s.y / 2, 0),
            vec3(0, s.y / 2, 0), vec3(0, 0, s.z / 2) }));

    // Front quad
    check(rayQuadIntersection(ray, { c + vec3(0, s.y / 2, s.z / 2),
            vec3(s.x / 2, 0, 0), vec3(0, s.y / 2, 0) }));

    // Back quad
    check(rayQuadIntersection(ray, { c + vec3(0, s.y / 2, -s.z / 2),
            vec3(s.x / 2, 0, 0), vec3(0, s.y / 2, 0) }));

    return intersection;
}

Intersection rayMapIntersection(Ray const& ray, Map const& map) {
    Intersection closest;

    for (auto& block : map.getBlocks()) {
        auto intersection = rayBlockIntersection(ray, block);
        if (!closest || (intersection &&
                intersection.get().first < closest.get().first))
            closest = intersection;
    } 

    return closest;
}

} // namespace game
