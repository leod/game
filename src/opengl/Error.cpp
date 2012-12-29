#include "opengl/Error.hpp"

#include <stdexcept>

#include <GL/glew.h>

namespace game {

void checkGLError(std::string const& location) {
    GLenum error = glGetError();

    if (error == GL_NO_ERROR)
       return; 

    throw std::runtime_error("OpenGL Error at " + location + ": " +
            std::string(reinterpret_cast<char const*>(gluErrorString(error))));
}

} // namespace game
