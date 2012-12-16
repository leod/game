#pragma once

#include <vector>
#include <map>

#include "core/Component.hpp"

namespace game {

struct System {
    System(Family family);
    virtual ~System();

    const Family getFamily() const;

    virtual void onRegister(Component*);
    virtual void onUnregister(Component*);

private:
    const Family family;
};

template<typename T> struct SystemBase {
    SystemBase()
        : System(T::staticGetFamily()) {
    }

    static Family staticGetFamily() {
        return T::staticGetFamily();
    }
};

typedef std::vector<System*> SystemList;
typedef std::map<Family, System*> SystemMap;

} // namespace game
