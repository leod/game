#include "opengl/Shader.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace game {

Shader::Shader(GLenum type, std::string const& filename) {
    {
        std::ifstream file(filename);

        if (!file.good())
            throw std::runtime_error("File " + filename + " not found.");

        std::stringstream ss;
        ss << filename;
        file.close();

        char const* source = ss.str().c_str();
        GLint sourceLength = ss.str().size();

        name = glCreateShader(type);
        glShaderSource(name, 1, &source, &sourceLength);
        glCompileShader(name);
    }

    {
        GLint shaderOk;
        glGetShaderiv(name, GL_COMPILE_STATUS, &shaderOk);
        if (!shaderOk) {
            GLint logLength;
            char* log;

            glGetShaderiv(name, GL_INFO_LOG_LENGTH, &logLength);
            log = static_cast<char*>(malloc(logLength));
            glGetShaderInfoLog(name, logLength, NULL, log);
            
            std::string str(log); 
            free(log);

            throw std::runtime_error(str);
        }
    }
}

Shader::~Shader() {
    glDeleteShader(name);
}

GLuint Shader::getName() const {
    return name;
}

} // namespace game
