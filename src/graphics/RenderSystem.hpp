#pragma once

#include "core/EntityRegistry.hpp"
#include "core/System.hpp"
#include "graphics/RenderComponent.hpp"

namespace game {

struct RenderSystem : public SystemBase<RenderComponent> {
    void render(ComponentItT<RenderComponent> begin,
                ComponentItT<RenderComponent> end);
};

} // namespace game
