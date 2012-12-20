#pragma once

#include <string>

#include <GL/glew.h>

namespace game {

struct Shader {
    Shader(GLenum type, std::string const& filename);
    ~Shader();

    GLuint getName() const;

    GLint getUniformLocation(char const*) const;
    GLint getAttribLocation(char const*) const;

private:
    GLuint name;
};

} // namespace game
