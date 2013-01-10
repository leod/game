#pragma once

#include <vector>

#include <boost/optional.hpp>

#include "math/Math.hpp"
#include "math/Intersection.hpp"

namespace game {

struct Map {
    // Creates a test map
    Map();

    // Very simplified map is good enough for now
    struct Block {
        vec3 groundCenter;
        float yRotation;
        vec3 scale;
        
        vec3 rotScale;

        Block(vec3, float, vec3);
    };

    std::vector<Block> const& getBlocks() const;

private:
    std::vector<Block> blocks;
};

Intersection rayMapIntersection(Ray const&, Map const&);

} // namespace game
