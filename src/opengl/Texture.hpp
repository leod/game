#pragma once

#include <string>

#include <GL/glew.h>

namespace game {

struct Texture {
    Texture();
    Texture(Texture&&);
    Texture(std::string const& filename);

    ~Texture();

    GLuint getName() const;
    void bind() const;

private:
    GLuint name;
};

} // namespace game
