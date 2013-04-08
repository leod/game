#pragma once

#include "math/Math.hpp"
#include "opengl/Buffer.hpp"

namespace game {

struct Map;
struct TextureManager;
struct ProgramManager;
struct VisionSystem;
struct Program;
struct Texture;

struct GroundRenderer {
    GroundRenderer(Map const&, TextureManager&, ProgramManager&,
                   VisionSystem&);

    void render(mat4 const& projection, mat4 const& view);

private:
    Map const& map;

    Texture* floorTexture;
    Program* program;

    VisionSystem& visionSystem;

    Buffer<vec3> quadVertices;
    Buffer<vec3> quadNormals;
    Buffer<vec2> quadTexCoords;
};

} // namespace game
