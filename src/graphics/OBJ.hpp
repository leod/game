#pragma once

#include <memory>
#include <vector>
#include <list>

#include "opengl/Buffer.hpp"
#include "math/Math.hpp"

namespace game {

struct Texture;
struct TextureManager;

// Loads a convenient subset of OBJ.
struct OBJ {
    struct Material {
        Texture* texture;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        Material();

        Material(Material const& a)
            : diffuse(a.diffuse) {

        }
    };

    struct Part {
        Material material;

        std::unique_ptr<Buffer<vec3>> vertices;
        std::unique_ptr<Buffer<vec2>> texcoords;
        std::unique_ptr<Buffer<vec3>> normals;

        Part(Material, Buffer<vec3>*, Buffer<vec2>*, Buffer<vec3>*); 
    };

    std::list<Part> parts;

    OBJ(std::string const& filename, TextureManager&);

private:
    typedef std::map<std::string, Material> MaterialLib;

    MaterialLib loadMaterialLib(std::string const& filename, TextureManager&);

    void load(std::string const& filename, TextureManager&);
};

} // namespace game
