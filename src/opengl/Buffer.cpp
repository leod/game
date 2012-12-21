#include "opengl/Buffer.hpp"

namespace game {

void drawElements(Buffer<unsigned short> const& buffer,
                  GLenum mode,
                  GLint count) {
    if (count == 0)
        count = buffer.getNumElements();

    buffer.bind(); 
    glDrawElements(mode, count, GL_UNSIGNED_SHORT, nullptr);
}

} // namespace game
