#include "math/Intersection.hpp"

#include <iostream>
#include <utility>

#include <boost/none.hpp>

using boost::none;

namespace game {

bool operator<=(Intersection a, Intersection b) {
    if (a) {
        if (b)
            return a.get() <= b.get();
        else
            return true;
    }

    return false;
}

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

    if (r >= 0 && glm::abs(s) <= 1 && glm::abs(t) <= 1)
        return r;
    else
        return none;
}

Intersection raySphereIntersection(Ray const& ray, Sphere const& sphere) {
    auto disp = ray.origin - sphere.center;

    // Coefficients of quadratic equation
    //     ar^2 + br + c = 0
    float a = glm::dot(ray.direction, ray.direction),
          b = 2 * glm::dot(disp, ray.direction),
          c = glm::dot(disp, disp) - sphere.radius * sphere.radius;

    // Discriminant of quadratic equation
    float d = b*b - 4*a*c;

    // Have solution?
    if (d < 0)
        return none;

    // Solve
    float x = (-b - sqrt(d)) / (2 * a);
    float y = (-b + sqrt(d)) / (2 * a);

    if (x > y)
        std::swap(x, y);

    if (y < 0)
        return none;

    if (x < 0)
        return y;

    return x;
}

} // namespace game
