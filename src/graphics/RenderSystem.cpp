#include "graphics/RenderSystem.hpp"

namespace game {

void RenderSystem::render(ComponentItT<RenderComponent> begin,
                          ComponentItT<RenderComponent> end) {
    for (auto it = begin; begin != end; ++it)
        (*it)->render();
}

} // namespace game
