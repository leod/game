#pragma once

#include <GL/glew.h>

#include "core/Error.hpp"
#include "math/Math.hpp"
#include "opengl/Error.hpp"
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
        Stride = sizeof(vec3)
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
    void unbind() const;

    GLuint getUniformLocation(char const*) const;
    GLuint getAttribLocation(char const*) const;

    void setUniform(GLuint location, GLint) const;
    void setUniform(GLuint location, GLfloat) const;
    void setUniform(GLuint location, vec2 const&) const;
    void setUniform(GLuint location, vec3 const&) const;
    void setUniform(GLuint location, vec4 const&) const;
    void setUniform(GLuint location, mat2 const&) const;
    void setUniform(GLuint location, mat3 const&) const;
    void setUniform(GLuint location, mat4 const&) const;

    template<typename T>
    void setAttrib(GLuint location, Buffer<T> const& buffer) {
        ASSERT(location < GL_MAX_VERTEX_ATTRIBS);
        ASSERT(detail::AttribInfos<T>::Size >= 1 &&
               detail::AttribInfos<T>::Size <= 4);
        ASSERT(detail::AttribInfos<T>::Stride >= 0);

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

    void unsetAttrib(GLuint location) {
        glDisableVertexAttribArray(location);
    }

private:
    GLint name;
};

} // namespace game
