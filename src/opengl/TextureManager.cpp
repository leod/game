#include "opengl/TextureManager.hpp"

#include "opengl/Texture.hpp"

namespace game {

Texture* TextureManager::load(std::string const& filename) {
    auto textureIt = textures.find(filename);
    if (textureIt != textures.end())
        return textureIt->second.get();

    Texture* texture = new Texture(filename);
    textures[filename] = std::unique_ptr<Texture>(texture);

    return texture;
}

} // namespace game
