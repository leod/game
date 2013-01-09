#include "map/Map.hpp"

#include <boost/none.hpp>

#include "math/Intersection.hpp"

using boost::none;

namespace game {

Map::Map()
    : blocks({
              { vec3(-7, 0, 0), 0, vec3(1, 2, 7) },
              { vec3(7, 0, 0), 0, vec3(1, 2, 7) },
              { vec3(0, 0, -15), 0, vec3(15, 3, 1) },
              { vec3(-5, 0, -8), 45, vec3(5, 1, 1) },
              { vec3(6, 0, -9), 135, vec3(5, 1, 1) }
             }) {
}

std::vector<Map::Block> const& Map::getBlocks() const {
    return blocks;
}

static Intersection
rayBlockIntersection(Ray const& ray, Map::Block const& block) {
    return none; 
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
