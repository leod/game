#include "world/ProjectileSystem.hpp"

#include "map/Map.hpp"
#include "physics/PhysicsComponent.hpp"
#include "net/NetComponent.hpp"
#include "world/PlayerComponent.hpp"

namespace game {

static bool canHit(ProjectileComponent* projectile, PlayerComponent* player) {
    // TODO: Use player fields once available instead of going to NetComponent
    auto net1 = projectile->getEntity()->component<NetComponent>();
    auto net2 = player->getEntity()->component<NetComponent>();

    if (!net1 || !net2) return false;

    return net1->getOwner() != net2->getOwner();
}

ProjectileSystem::ProjectileSystem(Map& map)
    : map(map) {
}

void ProjectileSystem::tick(bool localOnly) {
    std::vector<ProjectileComponent*> deleteMe;

    iterate([&] (ProjectileComponent* projectile) {
        if (localOnly && !projectile->isLocal())
            return;

        projectile->lifetime++;
        if (projectile->lifetime == 100 && false) {
            deleteMe.push_back(projectile);
            return;
        }

        auto physics = projectile->getPhysics();
        Ray ray = { physics->getPosition(), physics->getOrientation() };

        float velocity = 0.1; // TODO

        // Entity intersection
        Intersection entIntersection;
        Entity* entity = nullptr;
        
        {
            auto it = getManager()->familyBegin<PlayerComponent>();
            auto end = getManager()->familyEnd<PlayerComponent>();

            for (; it != end; ++it) {
                if (!canHit(projectile, *it))
                    continue;

                auto physics =
                    (*it)->getEntity()->component<PhysicsComponent>(); 
                ASSERT(physics);

                Sphere sphere = { physics->getPosition(), 0.3 };
                auto intersection = raySphereIntersection(ray, sphere);

                if (intersection <= entIntersection) {
                    entIntersection = intersection;
                    entity = (*it)->getEntity();

                    //TRACE(world) << entIntersection.get() << " - " << entity;
                }
            }
        }
        
        // Map intersection
        auto mapIntersection = rayMapIntersection(ray, map);

        // Pick closest intersection
        if (mapIntersection && mapIntersection.get() <= velocity &&
            mapIntersection <= entIntersection) {
            deleteMe.push_back(projectile);
        }
        else if (entIntersection && entIntersection.get() <= velocity &&
                 entIntersection <= mapIntersection) {
            ASSERT(entity);
            TRACE(world) << "Hit " << entity;
            deleteMe.push_back(projectile);
        }
        else {
            physics->setPosition(physics->getPosition()
                                 + physics->getOrientation() * velocity);
            //TRACE(world) << physics->getPosition();
        }
    });

    for (auto projectile : deleteMe)
        getManager()->remove(projectile->getEntity());
}

} // namespace game
