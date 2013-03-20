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
              { vec3(0, 0, -15), 0, vec3(15, 1, 1) }
             }) {
}

std::vector<Map::Block> const& Map::getBlocks() const {
    return blocks;
}

static Intersection
rayBlockIntersection(Ray const& ray, Map::Block const& block, Quad* outQuad) {
    // TODO: This is as naive as it gets :D
    Intersection intersection;

    auto c = block.groundCenter;
    auto s = block.scale;

    auto tryQuad = [&] (Quad const& quad) {
        auto i = rayQuadIntersection(ray, quad);

        if (!intersection || (i && i.get() < intersection.get())) {
            intersection = i;
            if (outQuad) *outQuad = quad;
        }
    };

    // Bottom quad
    tryQuad({ c,
              vec3(s.x / 2, 0, 0), vec3(0, 0, s.z / 2) });

    // Top quad
    tryQuad({ c + s.y,
              vec3(s.x / 2, 0, 0), vec3(0, 0, s.z / 2) });

    // Left quad
    tryQuad({ c + vec3(-s.x / 2, s.y / 2, 0),
              vec3(0, s.y / 2, 0), vec3(0, 0, s.z / 2) });

    // Right quad
    tryQuad({ c + vec3(s.x / 2, s.y / 2, 0),
              vec3(0, s.y / 2, 0), vec3(0, 0, s.z / 2) });

    // Front quad
    tryQuad({ c + vec3(0, s.y / 2, s.z / 2),
              vec3(s.x / 2, 0, 0), vec3(0, s.y / 2, 0) });

    // Back quad
    tryQuad({ c + vec3(0, s.y / 2, -s.z / 2),
              vec3(s.x / 2, 0, 0), vec3(0, s.y / 2, 0) });

    return intersection;
}

Intersection rayMapIntersection(Ray const& ray, Map const& map,
        Map::Block* outBlock, Quad* outQuad) {
    Intersection closest;
    Map::Block const* closestBlock = nullptr;
    Quad closestQuad;

    for (auto& block : map.getBlocks()) {
        Quad quad;
        auto intersection = rayBlockIntersection(ray, block, &quad);
        if (!closest || (intersection &&
                intersection.get() < closest.get())) {
            closest = intersection;
            closestBlock = &block;
            closestQuad = quad;
        }
    } 

    if (closestBlock && outBlock)
        *outBlock = *closestBlock;
    if (outQuad)
        *outQuad = closestQuad;

    return closest;
}

} // namespace game
