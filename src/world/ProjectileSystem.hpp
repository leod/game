#pragma once

#include "core/System.hpp"
#include "world/ProjectileComponent.hpp"

namespace game {

struct Map;

struct ProjectileSystem : public SystemBase<ProjectileComponent> {
    ProjectileSystem(Map&);

    void tick(bool localOnly);

private:
    Map& map;
};

} // namespace game
