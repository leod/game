#include "map/Map.hpp"

#include <iostream>

#include <boost/none.hpp>

#include "math/Intersection.hpp"
#include "util/Print.hpp"

using boost::none;

namespace game {

Map::Map()
    : blocks({
              /*{ vec3(-7, 0, 0), 0, vec3(1, 2, 7) },
              { vec3(7, 0, 0), 0, vec3(1, 2, 7) },
              { vec3(0, 0, -15), 0, vec3(15, 3, 1) },
              { vec3(-5, 0, -8), 45, vec3(5, 1, 1) },*/
              { vec3(6, 0, -9), 135, vec3(5, 1, 1) }
             }) {
}

std::vector<Map::Block> const& Map::getBlocks() const {
    return blocks;
}

static Intersection
rayBlockIntersection(Ray const& ray, Map::Block const& block) {
    Intersection intersection;
    std::string debug;
    std::string bestDebug;
    auto check = [&] (Intersection const& i) {
        if (!intersection || (i && i.get().first < intersection.get().first)) {
            intersection = i;
            bestDebug = debug;
        }
    };

    auto c = block.groundCenter;
    auto s = block.scale;

    s = vec3(glm::rotate(block.yRotation, vec3(0, 1, 0)) *
             vec4(s, 1.0));

    // Bottom quad
    debug = "bottom";
    check(rayQuadIntersection(ray, { c,
            vec3(s.x / 2, 0, 0), vec3(0, 0, s.z / 2) }));

    // Top quad
    debug = "top";
    check(rayQuadIntersection(ray, { c + s.y,
            vec3(s.x / 2, 0, 0), vec3(0, 0, s.z / 2) }));

    // Left quad
    debug = "left";
    check(rayQuadIntersection(ray, { c + vec3(-s.x / 2, s.y / 2, 0),
            vec3(0, s.y / 2, 0), vec3(0, 0, s.z / 2) }));

    // Right quad
    debug = "right";
    check(rayQuadIntersection(ray, { c + vec3(s.x / 2, s.y / 2, 0),
            vec3(0, s.y / 2, 0), vec3(0, 0, s.z / 2) }));

    // Front quad
    debug = "front";
    check(rayQuadIntersection(ray, { c + vec3(0, s.y / 2, s.z / 2),
            vec3(s.x / 2, 0, 0), vec3(0, s.y / 2, 0) }));

    // Back quad
    debug = "back";
    check(rayQuadIntersection(ray, { c + vec3(0, s.y / 2, -s.z / 2),
            vec3(s.x / 2, 0, 0), vec3(0, s.y / 2, 0) }));

    if (intersection) {
        Quad quad = { c + vec3(0, s.y / 2, -s.z / 2),
            vec3(s.x / 2, 0, 0), vec3(0, s.y / 2, 0) };
        std::cout << quad.center << ", " << quad.u << ", " << quad.v << std::endl;
        std::cout << "ray " << ray.origin << ", " << ray.direction << " hit " << bestDebug << " at " << intersection.get().second << std::endl;
    }

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
