#include "core/EntityManager.hpp"

#include <algorithm>

#include "core/Entity.hpp"
#include "core/System.hpp"
#include "core/Error.hpp"

namespace game {

EntityManager::EntityManager(std::vector<SystemBase*> const& systems)
    : biggestId() {
    for (auto s : systems) {
        // TODO: See if this limitation makes sense.
        ASSERT_MSG(this->systems.find(s->getFamilyId()) == this->systems.end(),
                   "Can't have two systems in the same family.");

        this->systems[s->getFamilyId()] = s;
        families[s->getFamilyId()] = ComponentList();

        ASSERT(s->entities == nullptr);
        s->entities = this;
    } 
}

Entity* EntityManager::get(EntityId id) {
    return entities[id];
}

Entity const* EntityManager::get(EntityId id) const {
    auto it = entities.find(id);

    ASSERT_MSG(it != entities.end(), "Entity #" << id << " not found.");

    return it->second;
}

Entity* EntityManager::create(ComponentList&& components) {
    // Create and register new entity
    EntityId id = ++biggestId;
    Entity* entity = new Entity(id, std::move(components), this);

    for (auto component : entity->components)
        component->setOwner(entity);

    entities[id] = entity;

    // Notify the systems about our new components and register
    // the components in their family list
    for (auto component : entity->components) {
        families[component->getFamilyId()].push_back(component);

        auto it = systems.find(component->getFamilyId());
        if (it != systems.end())
            it->second->onRegister(component);
    }

    return entity;
}

void EntityManager::remove(Entity* entity) {
    auto it = entities.find(entity->id);

    ASSERT_MSG(it != entities.end(),
               "Entity #" << entity->id << " not found in registry.");

    for (auto component : entity->components) {
        // Notify corresponding system
        {
            auto it = systems.find(component->getFamilyId()); 
            if (it != systems.end())
                it->second->onUnregister(component);
        }

        // Remove from family list
        {
            auto& family = families[component->getFamilyId()];

            // TODO: Use std::list or something else for the families?
            //       Could even store links in the components.
            auto it = std::find(family.begin(), family.end(), component);
            ASSERT(it != family.end());

            family.erase(it);
        }
    }

    entities.erase(it);
    delete entity;
}

SystemBase* EntityManager::system(FamilyId familyId) {
    return systems[familyId];
}

SystemBase const* EntityManager::system(FamilyId familyId) const {
    return systems.find(familyId)->second;
}

} // namespace game
