#include "physics/PhysicsSystem.hpp"

#include "core/Log.hpp"

#include "map/Map.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

PhysicsSystem::PhysicsSystem(Map& map)
    : map(map) {
}

static void
tryMove(Map const& map, PhysicsComponent* physics, vec3 const& delta) {
    Ray ray = { physics->getPosition(), delta };
    auto intersection = rayMapIntersection(ray, map, nullptr, nullptr);

    if (!intersection || intersection.get() > 1) {
        // No collision
        physics->setPosition(physics->getPosition() + delta);
    } else {
        // Collision - walk as far as we can
        float r = intersection.get() - 0.001;
        if (r < 0) r = 0;

        physics->setPosition(physics->getPosition() + r * delta);
    }
}

void PhysicsSystem::moveOne(PhysicsComponent* physics, vec3 const& delta) {
    Ray ray = { physics->getPosition(), delta };

    Quad quad;
    auto intersection = rayMapIntersection(ray, map, nullptr, &quad);

    if (!intersection || intersection.get() > 1) {
        // No collision
        physics->setPosition(physics->getPosition() + delta);
    } else {
        auto n = quad.n();

        // Collision with a surface having normal vector n.
        // Decompose delta into a part parallel to n and a part
        // orthogonal to n
        vec3 p = glm::dot(delta, n) * n;
        vec3 o = delta - p;

        // Try moving in the directions of just the parts
        tryMove(map, physics, p);
        tryMove(map, physics, o);
    }
}

} // namespace game
