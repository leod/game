#pragma once

#include <string>
#include <map>
#include <memory>

namespace game {

struct Texture;

struct TextureManager {
    Texture* load(std::string const& filename);

private:
    std::map<std::string, std::unique_ptr<Texture>> textures;
};

} // namespace game
