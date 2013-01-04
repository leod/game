#include "core/EntityRegistry.hpp"

#include "core/Entity.hpp"
#include "core/System.hpp"
#include "core/Error.hpp"

namespace game {

EntityRegistry::EntityRegistry(SystemList systems)
    : biggestId() {
    for (auto s : systems) {
        // TODO: See if this limitation makes sense.
        ASSERT_MSG(this->systems.find(s->getFamilyId()) == this->systems.end(),
                   "Can't have two systems in the same family.");

        this->systems[s->getFamilyId()] = s;
        families[s->getFamilyId()] = ComponentList();

        ASSERT(s->registry == nullptr);
        s->registry = this;
    } 
}

Entity* EntityRegistry::get(EntityId id) {
    return entities[id];
}

Entity const* EntityRegistry::get(EntityId id) const {
    auto it = entities.find(id);

    ASSERT_MSG(it != entities.end(),
               "Entity #" << id << " not found.");

    return it->second;
}

Entity* EntityRegistry::add(ComponentList components) {
    // Convert component list to map
    ComponentMap map;

    for (auto c : components) {
        ASSERT_MSG(map.find(c->getFamilyId()) == map.end(),
                   "Can't have two components in the same family.");
 
        map[c->getFamilyId()] = c;
    } 

    // Create and register new entity
    EntityId id = biggestId++;
    Entity* entity = new Entity(id, map, this);

    for (auto c : components)
        c->setOwner(entity);

    entities[id] = entity;

    // Notify the systems about our new components and register
    // the components in their family list
    for (auto c : components) {
        families[c->getFamilyId()].push_back(c);

        auto it = systems.find(c->getFamilyId());
        if (it != systems.end())
            it->second->onRegister(c);
    }

    return entity;
}

System* EntityRegistry::system(FamilyId familyId) {
    return systems[familyId];
}

System const* EntityRegistry::system(FamilyId familyId) const {
    return systems.find(familyId)->second;
}

} // namespace game
