#include "graphcis/RenderSystem.hpp"

namespace game {

void RenderSystem::render(ComponentListT<RenderComponent*> cs) {
    for (auto c : cs)
        c->render();
}

} // namespace game
