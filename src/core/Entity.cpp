#include "core/Entity.hpp"

#include <algorithm>

namespace game {

Entity::~Entity() {
    for (auto component : components)
        delete component;
}

ComponentBase* Entity::component(FamilyId familyId) {
    auto it = std::find_if(components.begin(), components.end(),
            [familyId] (ComponentBase* component) {
                return component->getFamilyId() == familyId;
            });
    return it != components.end() ? *it : nullptr;
}

ComponentBase const* Entity::component(FamilyId familyId) const {
    auto it = std::find_if(components.begin(), components.end(),
            [familyId] (ComponentBase const* component) {
                return component->getFamilyId() == familyId;
            });
    return it != components.end() ? *it : nullptr;
}

EntityId Entity::getId() const {
    return id;
}

EntityManager* Entity::getManager() {
    return entities;
}

EntityManager const* Entity::getManager() const {
    return entities;
}

Entity::Entity(EntityId id, ComponentList&& components,
               EntityManager* entities)
    : id(id), components(components), entities(entities) {
}

std::ostream& operator<<(std::ostream& os, Entity const* entity) {
    if (entity) {
        os << "Entity " << entity->getId() << "[";

        size_t i = 0;
        for (auto component : entity->components) {
            os << component;

            if (++i != entity->components.size())
                os << ", ";
        }

        os << "]";
    }
    else os << "nullptr";

    return os;
}

} // namespace game
