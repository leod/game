#pragma once

#include "core/System.hpp"
#include "math/Math.hpp"

namespace game {

struct Map;
struct PhysicsComponent;

struct PhysicsSystem : public System<PhysicsComponent> {
    PhysicsSystem(Map&);

    void moveOne(PhysicsComponent*, vec3 const& delta);

private:
    Map& map;
};

} // namespace game
