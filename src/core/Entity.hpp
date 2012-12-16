#pragma once

#include "core/Component.hpp"

namespace game {

typedef int EntityId;

struct EntityRegistry;

// An entity is an ID and a list of components.
// An entity can have at most one component of each family.
struct Entity {
    Entity(EntityId id, ComponentMap components, EntityRegistry* registry)
        : id(id), components(components), registry(registry) {
    }

    Component* component(Family family) {
        auto it = components.find(family);
        return it != components.end() ? it->second : nullptr;
    }

    template<typename T> T* component() {
        return dynamic_cast<T*>(component(T::staticGetFamily()));
    }

    const Component* component(Family family) const {
        auto it = components.find(family);
        return it != components.end() ? it->second : nullptr;
    }

    template<typename T> const T* component() const {
        return dynamic_cast<T*>(component(T::staticGetFamily()));
    }

    const EntityId getId() const {
        return id;
    }

    EntityRegistry* getRegistry() {
        return registry;
    }

    const EntityRegistry* getRegistry() const {
        return registry;
    }

private:
    const EntityId id;
    const ComponentMap components;

    EntityRegistry* registry;
};

} // namespace game
