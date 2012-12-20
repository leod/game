#include "opengl/Error.hpp"

#include <string>
#include <stdexcept>

#include <GL/glew.h>

namespace game {

void checkGLError() {
    GLenum error = glGetError();

    if (error == GL_NO_ERROR)
       return; 

    throw new std::runtime_error(
            reinterpret_cast<char const*>(gluErrorString(error)));
}

} // namespace game
