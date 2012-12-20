#include "world/TickComponent.hpp"

namespace game {

TickComponents::TickComponents(ComponentListT<TickComponent*> components)
    : components(components) {
}

void TickComponents::tick() {
    for (auto component : components)
        (*component)->tick();
}

} // namespace game
