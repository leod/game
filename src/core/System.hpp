#pragma once

#include <vector>
#include <map>
#include <functional>

#include "core/Error.hpp"
#include "core/Component.hpp"
#include "core/EntityRegistry.hpp"

namespace game {

struct System {
    friend class EntityRegistry;

    System(FamilyId);
    virtual ~System();

    virtual void onRegister(Component*);
    virtual void onUnregister(Component*);

    FamilyId getFamilyId() const;
    EntityRegistry* getEntities();
    EntityRegistry const* getEntities() const;

private:
    FamilyId const familyId;

    EntityRegistry* entities;
};

template<typename T> struct SystemBase : public System {
    SystemBase()
        : System(T::staticGetFamilyId()) {
    }

    void onRegister(Component* c) {
        T* t = dynamic_cast<T*>(c);
        ASSERT(t != nullptr);
        onRegister(t);
    }

    void onUnregister(Component* c) {
        T* t = dynamic_cast<T*>(c);
        ASSERT(t != nullptr);
        onUnregister(t);
    }

    virtual void onRegister(T*) {
    }

    virtual void onUnregister(T*) {
    }

    static FamilyId staticGetFamilyId() {
        return T::staticGetFamilyId();
    }

    void iterate(std::function<void(T*)> const& f) {
        EntityRegistry* registry = getEntities();
        auto it = registry->familyBegin<T>();
        auto end = registry->familyEnd<T>();

        for (; it != end; ++it) f(*it);
    }

    void iterate(std::function<void(T const*)> const& f) const {
        EntityRegistry const* registry = getEntities();
        auto it = registry->familyBegin<T>();
        auto end = registry->familyEnd<T>();

        for (; it != end; ++it) f(*it);
    }
};

} // namespace game
