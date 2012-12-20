#pragma once

#include <string>
#include <map>
#include <memory>

#include "opengl/Texture.hpp"

namespace game {

struct TextureManager {
    Texture* load(std::string const& filename);

private:
    std::map<std::string, std::unique_ptr<Texture>> textures;
};

} // namespace game
