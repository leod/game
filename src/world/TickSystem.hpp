#pragma once

#include "core/System.hpp"
#include "world/TickComponent.hpp"

namespace game {

struct TickSystem : public System<TickComponent> {
    void tick();
};

} // namespace game
