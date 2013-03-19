#include "core/Entity.hpp"

#include <algorithm>

namespace game {

Entity::~Entity() {
    for (auto component : components)
        delete component;
}

Component* Entity::component(FamilyId familyId) {
    auto it = std::find_if(components.begin(), components.end(),
            [familyId] (Component* component) {
                return component->getFamilyId() == familyId;
            });
    return it != components.end() ? *it : nullptr;
}

Component const* Entity::component(FamilyId familyId) const {
    auto it = std::find_if(components.begin(), components.end(),
            [familyId] (Component const* component) {
                return component->getFamilyId() == familyId;
            });
    return it != components.end() ? *it : nullptr;
}

EntityId Entity::getId() const {
    return id;
}

EntityManager* Entity::getEntities() {
    return entities;
}

EntityManager const* Entity::getEntities() const {
    return entities;
}

Entity::Entity(EntityId id, ComponentList&& components,
               EntityManager* entities)
    : id(id), components(components), entities(entities) {
}

std::ostream& operator<<(std::ostream& os, Entity const* entity) {
    if (entity) {
        os << "Entity#" << entity->getId() << "{";

        for (auto component : entity->components) {
            os << component;
            os << ":" << component->getFamilyId();
            os << ",";
        }
        os << "}";
    }
    else os << "nullptr";

    return os;
}

} // namespace game
