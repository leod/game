#pragma once

#include "core/System.hpp"
#include "opengl/Texture.hpp"
#include "opengl/Framebuffer.hpp"
#include "graphics/VisionComponent.hpp"

namespace game {

struct Map;
struct ProgramManager;
struct Program;

struct VisionSystem : public SystemBase<VisionComponent> {
    VisionSystem(Map const&, ProgramManager&);

    void renderVision(mat4 const& projection, mat4 const& view);
    Texture const& getTexture() const;

private:
    Map const& map; 
    Program* program;
    
    Framebuffer framebuffer;
};

} // namespace game
