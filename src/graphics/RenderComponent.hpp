#pragma once

#include "core/Component.hpp"

namespace game {

struct RenderComponent : public ComponentFamily<RenderComponent> {
    virtual void render() = 0;
};

} // namespace game
