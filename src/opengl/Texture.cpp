#include "opengl/Texture.hpp"

#include <iostream>

#include <boost/scoped_array.hpp>

#include <SFML/Graphics/Image.hpp>

#include "core/Error.hpp"

namespace game {

Texture::Texture() {
    glGenTextures(1, &name);
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

void Texture::saveToFile(std::string const& filename) const {
    glBindTexture(GL_TEXTURE_2D, name);
    
    GLint width, height;
    GLint format;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT,
            &format);
    ASSERT(format == GL_RGBA);

    boost::scoped_array<GLubyte> buffer(new GLubyte[width * height * 4]);
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, buffer.get());

    sf::Image image;
    image.create(width, height, buffer.get());
    image.saveToFile(filename);

    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace game
