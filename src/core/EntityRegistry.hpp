#pragma once

#include "core/Entity.hpp"
#include "core/System.hpp"

namespace game {

typedef std::map<EntityId, Entity*> EntityMap;

struct EntityRegistry {
    EntityRegistry(SystemList systems);

    Entity* add(ComponentList);
    Entity* get(EntityId);
    const Entity* get(EntityId) const;

    ComponentList family(Family);
    const ComponentList family(Family) const;

    System* system(Family);
    const System* system(Family) const;

private:
    SystemMap systems;

    EntityMap entities;
    EntityId biggestId;
};

} // namespace game
