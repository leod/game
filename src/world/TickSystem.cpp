#include "world/TickSystem.hpp"

namespace game {

void TickSystem::tick() {
    iterate([&] (TickComponent* component) {
        component->tick();
    });
}

} // namespace game
