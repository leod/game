#include "world/ProjectileSystem.hpp"

#include "map/Map.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

ProjectileSystem::ProjectileSystem(Map& map)
    : map(map) {
}

void ProjectileSystem::tick(bool localOnly) {
    std::vector<ProjectileComponent*> deleteMe;

    iterate([&] (ProjectileComponent* projectile) {
        if (localOnly && !projectile->isLocal())
            return;

        auto physics = projectile->getPhysics();
        Ray ray = { physics->getPosition(), physics->getOrientation() };

        float velocity = 3; // TODO
        
        auto intersection = rayMapIntersection(ray, map);

        if (intersection && intersection.get().first <= velocity)
            deleteMe.push_back(projectile);
        else
            physics->setPosition(physics->getPosition()
                                 + physics->getOrientation() * velocity);
    });

    for (auto projectile : deleteMe)
        getEntities()->remove(projectile->getEntity());
}

} // namespace game
