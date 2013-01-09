#pragma once

#include <utility>

#include <boost/optional.hpp>

#include "math/Math.hpp"

namespace game {

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Quad {
    vec3 center;
    vec3 u;
    vec3 v;
};

typedef boost::optional<std::pair<float, vec3>> Intersection;

Intersection rayQuadIntersection(Ray const&, Quad const&);

} // namespace game
