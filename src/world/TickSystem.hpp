#pragma once

#include "core/System.hpp"
#include "world/TickComponent.hpp"

namespace game {

struct TickSystem : public SystemBase<TickComponent> {
    void tick(ComponentItT<TickComponent> begin,
              ComponentItT<TickComponent> end);
};

} // namespace game
