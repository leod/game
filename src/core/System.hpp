#pragma once

#include <vector>
#include <map>

#include "core/Component.hpp"

namespace game {

struct System {
    System(FamilyId familyId);
    virtual ~System();

    FamilyId getFamilyId() const;

    virtual void onRegister(Component*);
    virtual void onUnregister(Component*);

private:
    FamilyId const familyId;
};

template<typename T> struct SystemBase : public System {
    SystemBase()
        : System(T::staticGetFamilyId()) {
    }

    static FamilyId staticGetFamilyId() {
        return T::staticGetFamilyId();
    }
};

} // namespace game
