#include "world/TickSystem.hpp"

namespace game {

void TickSystem::tick(ComponentItT<TickComponent> begin,
                      ComponentItT<TickComponent> end) {
    for (auto it = begin; it != end; ++it)
        (*it)->tick();
}

} // namespace game
