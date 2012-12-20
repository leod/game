#pragma once

#include <GL/glew.h>

#include "math/Math.hpp"

namespace game {

struct Shader;

struct Program {
    Program(Shader const* vertex,
            Shader const* fragment,
            Shader const* geometry = nullptr);
    ~Program();

    GLint getName() const;
    void bind() const;

    GLint getUniformLocation(char const*) const;
    GLint getAttribLocation(char const*) const;

    void setUniform(GLint location, GLint) const;
    void setUniform(GLint location, GLfloat) const;
    void setUniform(GLint location, vec2 const&) const;
    void setUniform(GLint location, vec3 const&) const;
    void setUniform(GLint location, vec4 const&) const;
    void setUniform(GLint location, mat2 const&) const;
    void setUniform(GLint location, mat3 const&) const;
    void setUniform(GLint location, mat4 const&) const;

private:
    GLint name;
};

} // namespace game
