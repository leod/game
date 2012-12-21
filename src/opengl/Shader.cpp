#include "opengl/Shader.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>

namespace game {

Shader::Shader(GLenum type, std::string const& filename) {
    {
        std::ifstream file(filename, std::ios::in);

        if (!file)
            throw std::runtime_error("File " + filename + " not found.");

        // Read whole file into contents
        std::string contents;
        file.seekg(0, std::ios::end);
        contents.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&contents[0], contents.size());
        file.close();

        char const* source = contents.c_str();
        GLint sourceLength = contents.size();

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

            throw std::runtime_error("Error in " + filename + ": " + str);
        }
    }
}

Shader::~Shader() {
    glDeleteShader(name);
}

GLuint Shader::getName() const {
    return name;
}

GLint Shader::getType() const {
    GLint type;
    glGetShaderiv(name, GL_SHADER_TYPE, &type);
    return type;
}

} // namespace game
