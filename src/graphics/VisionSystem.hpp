#pragma once

#include "core/System.hpp"
#include "graphics/VisionComponent.hpp"

namespace game {

struct Map;
struct ProgramManager;
struct Program;

struct VisionSystem : public SystemBase<VisionComponent> {
    VisionSystem(Map const&, ProgramManager&);

    void renderVision(mat4 const& projection, mat4 const& view);

private:
    Map const& map; 
    Program* program;
};

} // namespace game
