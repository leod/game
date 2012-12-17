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

    ComponentList family(FamilyId);
    const ComponentList family(FamilyId) const;

    System* system(FamilyId);
    const System* system(FamilyId) const;

private:
    SystemMap systems;

    EntityMap entities;
    EntityId biggestId;
};

} // namespace game
