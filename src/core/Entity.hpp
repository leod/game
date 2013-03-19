#pragma once

#include <ostream>

#include "core/Component.hpp"

namespace game {

typedef int EntityId;

template<typename T>
using ComponentListT = std::vector<T*>;
typedef ComponentListT<Component> ComponentList;

struct EntityManager;

// An entity is an Id and a list of components.
// An entity can have at most one component of each family.
struct Entity {
    friend struct Component;
    friend struct EntityManager;

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
    EntityManager* getManager();
    EntityManager const* getManager() const;

private:
    // Only EntityManager ever constructs Entities.
    Entity(EntityId, ComponentList&&, EntityManager*);

    EntityId const id;
    ComponentList const components;

    EntityManager* const entities;

    friend std::ostream& operator<<(std::ostream&, Entity const*);
};

std::ostream& operator<<(std::ostream&, Entity const*);

} // namespace game
