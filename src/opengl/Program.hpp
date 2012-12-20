#pragma once

#include <GL/glew.h>

#include "math/Math.hpp"
#include "opengl/Buffer.hpp"

namespace game {

struct Shader;

namespace detail {

template<typename T> struct AttribInfos {
};

template<> struct AttribInfos<vec2> {
    enum {
        Size = 2,
        Type = GL_FLOAT,
        Stride = sizeof(vec2)
    };
};

template<> struct AttribInfos<vec3> {
    enum {
        Size = 3,
        Type = GL_FLOAT,
        Stride = sizeof(vec2)
    };
};

} // namespace detail

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

    template<typename T>
    void setAttribs(GLint location, Buffer<T> const& buffer) {
        buffer.bind();
        glVertexAttribPointer(
            location, 
            detail::AttribInfos<T>::Size,
            detail::AttribInfos<T>::Type,
            GL_FALSE,
            detail::AttribInfos<T>::Stride,
            nullptr);
        glEnableVertexAttribArray(location);
    }

    void unsetAttribs(GLint location) {
        glDisableVertexAttribArray(location);
    }

private:
    GLint name;
};

} // namespace game
