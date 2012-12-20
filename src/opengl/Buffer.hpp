#pragma once

#include <vector>
#include <GL/glew.h>

namespace game {

template<typename T>
struct Buffer {
    Buffer(GLenum target,
           std::vector<T> const&,
           GLenum usage = GL_STATIC_DRAW);
    ~Buffer();

    GLuint getName() const;
    void bind() const;

private:
    GLenum target;
    GLuint name;
};

void drawElements(Buffer<unsigned short> const&,
                  GLenum mode,
                  GLint count);

template<typename T>
Buffer<T>::Buffer(GLenum target, std::vector<T> const& data, GLenum usage)
    : target(target) {
    glGenBuffers(1, &name);
    glBindBuffer(target, name);
    glBufferData(target, sizeof(T) * data.size(), &data[0], usage);
}

template<typename T>
Buffer<T>::~Buffer() {
    glDeleteBuffers(1, &name);
}

template<typename T>
GLuint Buffer<T>::getName() const {
    return name;
}

template<typename T>
void Buffer<T>::bind() const {
    glBindBuffer(target, name);
}

} // namespace game
