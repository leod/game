#pragma once

#include "math/Math.hpp"
#include "opengl/Buffer.hpp"

namespace game {

struct Map;
struct TextureManager;
struct ProgramManager;
struct VisionSystem;
struct Program;

struct MapRenderer {
    MapRenderer(Map const&, TextureManager&, ProgramManager&, VisionSystem&);

    void render(mat4 const& projection, mat4 const& view);

private:
    Map const& map;

    Program* program;

    VisionSystem& vision;

    Buffer<vec3> cubePositions;
    Buffer<vec3> cubeNormals;
};

} // namespace game
