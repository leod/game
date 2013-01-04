#pragma once

#include <vector>
#include <map>

#include "core/Component.hpp"
#include "core/EntityRegistry.hpp"

namespace game {

struct System {
    System(FamilyId);
    virtual ~System();

    FamilyId getFamilyId() const;

    virtual void onRegister(Component*);
    virtual void onUnregister(Component*);

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
        for (auto it = registry->familyBegin<T>();
             it != registry->familyEnd<T>();
             ++it) {
            f(*it);
        }
    }

    void iterate(std::function<void(T*)> const& f) const {
        for (auto it = registry->familyBegin<T>();
             it != registry->familyEnd<T>();
             ++it) {
            f(*it);
        }
    }
};

} // namespace game
