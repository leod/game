#include "graphcis/RenderSystem.hpp"

namespace game {

void RenderSystem::render(ComponentIt<RenderComponent> begin,
                          ComponentIt<RenderComponent> end);
    for (auto it = begin; begin != end; ++it)
        it->render();
}

} // namespace game
