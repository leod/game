#pragma once

#include "core/System.hpp"

namespace game {

struct RenderSystem : public System<RenderComponent> {
    void render(ComponentListT<RenderComponent*> cs);    
};

} // namespace game
