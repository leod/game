#pragma once

#include <vector>
#include <map>

#include "core/Component.hpp"

namespace game {

struct System {
    System(FamilyId familyId);
    virtual ~System();

    const FamilyId getFamilyId() const;

    virtual void onRegister(Component*);
    virtual void onUnregister(Component*);

private:
    const FamilyId familyId;
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
