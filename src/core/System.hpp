#pragma once

#include <vector>
#include <map>
#include <functional>

#include "core/Component.hpp"
#include "core/EntityRegistry.hpp"

namespace game {

struct System {
    System(FamilyId);
    virtual ~System();

    virtual void onRegister(Component*);
    virtual void onUnregister(Component*);

    FamilyId getFamilyId() const;
    EntityRegistry* getRegistry();
    EntityRegistry const* getRegistry() const;

private:
    FamilyId const familyId;

    EntityRegistry* registry;
    friend class EntityRegistry;
};

template<typename T> struct SystemBase : public System {
    SystemBase()
        : System(T::staticGetFamilyId()) {
    }

    static FamilyId staticGetFamilyId() {
        return T::staticGetFamilyId();
    }

    void iterate(std::function<void(T*)> const& f) {
        EntityRegistry* registry = getRegistry();
        auto it = registry->familyBegin<T>();
        auto end = registry->familyEnd<T>();

        for (; it != end; ++it) f(*it);
    }

    void iterate(std::function<void(T const*)> const& f) const {
        EntityRegistry const* registry = getRegistry();
        auto it = registry->familyBegin<T>();
        auto end = registry->familyEnd<T>();

        for (; it != end; ++it) f(*it);
    }
};

} // namespace game
