#pragma once

#include "core/Component.hpp"

namespace game {

typedef int EntityId;
typedef std::map<FamilyId, Component*> ComponentMap;

struct EntityRegistry;

// An entity is an ID and a list of components.
// An entity can have at most one component of each family.
struct Entity {
    friend class Component;

    Entity(EntityId id, ComponentMap components, EntityRegistry* registry)
        : id(id), components(components), registry(registry) {
    }

    Component* component(FamilyId familyId) {
        auto it = components.find(familyId);
        return it != components.end() ? it->second : nullptr;
    }

    template<typename T> T* component() {
        return dynamic_cast<T*>(component(T::staticGetFamilyId()));
    }

    Component const* component(FamilyId familyId) const {
        auto it = components.find(familyId);
        return it != components.end() ? it->second : nullptr;
    }

    template<typename T> T const* component() const {
        return dynamic_cast<T*>(component(T::staticGetFamilyId()));
    }

    EntityId const getId() const {
        return id;
    }

    EntityRegistry* getRegistry() {
        return registry;
    }

    EntityRegistry const* getRegistry() const {
        return registry;
    }

private:
    EntityId const id;
    ComponentMap const components;

    EntityRegistry* registry;
};

} // namespace game
