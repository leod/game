#include "opengl/Texture.hpp"

#include <SFML/Graphics/Image.hpp>

namespace game {

Texture::Texture(std::string const& filename) {
    glGenTextures(1, &name);
    glBindTexture(GL_TEXTURE_2D, name);

    sf::Image image;
    image.loadFromFile(filename);

    glTexImage2D(GL_TEXTURE_2D,
                 0, // LOD
                 GL_RGBA8,
                 image.getSize().x,
                 image.getSize().y,
                 0, // border
                 GL_RGBA, // TODO: We shouldn't always use RGBA for textures.
                 GL_UNSIGNED_BYTE,
                 image.getPixelsPtr());
}

Texture::~Texture() {
    glDeleteTextures(1, &name);
}

GLuint Texture::getName() const {
    return name;
}

} // namespace game
