#include "math/Intersection.hpp"

#include <iostream>

#include <boost/none.hpp>

using boost::none;

namespace game {

Intersection rayQuadIntersection(Ray const& ray, Quad const& quad) {
    // TODO: This is as naive as it gets :D

    // ray(r) = quad(s, t)
    // ray.origin + r * ray.direction = quad.center + s * quad.u + t * quad.v
    // quad.center - ray.origin = r * ray.direction - s * quad.u - t * quad.v
    // quad.center - ray.origin = {ray.direction, -quad.u, -quad.v} * [r, s, t]

    mat3 m(ray.direction, -quad.u, -quad.v);
    vec3 b(quad.center - ray.origin);

    mat3 mInv = glm::inverse(m);
    vec3 x = mInv * b;
    float r = x.x, s = x.y, t = x.z;

    vec3 p = ray.origin + r * ray.direction;

    if (r >= 0 && glm::abs(s) <= 1 && glm::abs(t) <= 1)
        return std::make_pair(r, p);
    else
        return none;
}

} // namespace game
