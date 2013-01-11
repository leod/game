#include "opengl/Texture.hpp"

#include <SFML/Graphics/Image.hpp>

namespace game {

Texture::Texture() {
    glGenTextures(1, &name);
}

Texture::Texture(Texture&& texture) {
    name = texture.name;

    // For debugging: invalidate the other texture
    texture.name = -1;
}

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

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, name);
}

} // namespace game
