#pragma once

#include "core/Component.hpp"
#include "core/EntityRegistry.hpp"

namespace game {

struct TickComponent : public ComponentFamily<TickComponent> {
    virtual void tick() = 0;
};

struct TickComponents : public TickComponent {
    TickComponents(ComponentListT<TickComponent*>);
    void tick();

private:
    ComponentListT<TickComponent*> components;
};

} // namespace game
