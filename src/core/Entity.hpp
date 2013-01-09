#pragma once

#include "core/Component.hpp"

namespace game {

typedef int EntityId;

template<typename T>
using ComponentListT = std::vector<T*>;
typedef ComponentListT<Component> ComponentList;

struct EntityRegistry;

// An entity is an Id and a list of components.
// An entity can have at most one component of each family.
struct Entity {
    friend class Component;
    friend class EntityRegistry;

    ~Entity();

    Component* component(FamilyId);
    Component const* component(FamilyId) const;

    template<typename T> T* component() {
        return dynamic_cast<T*>(component(T::staticGetFamilyId()));
    }

    template<typename T> T const* component() const {
        return dynamic_cast<T const*>(component(T::staticGetFamilyId()));
    }

    EntityId getId() const;
    EntityRegistry* getEntities();
    EntityRegistry const* getEntities() const;

private:
    // Only EntityRegistry ever constructs Entities.
    Entity(EntityId, ComponentList&&, EntityRegistry*);

    EntityId const id;
    ComponentList const components;

    EntityRegistry* const entities;
};

} // namespace game
