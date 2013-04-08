#pragma once

#include <utility>

#include <boost/optional.hpp>

#include "math/Math.hpp"
#include "math/BoundingBox.hpp"

namespace game {

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Quad {
    vec3 center;
    vec3 u;
    vec3 v;

    vec3 n() const {
        return glm::normalize(glm::cross(u, v));
    }
};

struct Sphere {
    vec3 center;
    float radius;
};

typedef boost::optional<float> Intersection; // Only ray intersections for now

// Returns true iff
//     a != none and b == none or
//     a != none and b != none and a < b,
// so it's a nice way to check if an intersection exists and is closer
// than another intersection.
bool operator<(Intersection a, Intersection b);

Intersection rayQuadIntersection(Ray const&, Quad const&);
Intersection raySphereIntersection(Ray const&, Sphere const&);
Intersection rayBoundingBoxIntersection(Ray const&, BoundingBox const&,
                                        vec3* outNormal = nullptr);

} // namespace game
