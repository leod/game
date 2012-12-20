#include "opengl/Buffer.hpp"

namespace game {

void drawElements(Buffer<unsigned short> const& buffer,
                  GLenum mode,
                  GLint count) {
    buffer.bind(); 
    glDrawElements(mode, count, GL_UNSIGNED_SHORT, nullptr);
}

} // namespace game
