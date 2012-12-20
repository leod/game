#include "opengl/Program.hpp"

#include <stdexcept>

#include "core/Error.hpp"
#include "opengl/Shader.hpp"

namespace game {

Program::Program(Shader const* vertex,
                 Shader const* fragment,
                 Shader const* geometry) {
    ASSERT_MSG(vertex != nullptr, "Vertex shader is required.");
    ASSERT_MSG(fragment != nullptr, "Fragment shader is required.");

    name = glCreateProgram();
    glAttachShader(name, vertex->getName());
    glAttachShader(name, fragment->getName());
    if (geometry) glAttachShader(name, geometry->getName());

    {
        GLint programOk;
        glGetProgramiv(name, GL_LINK_STATUS, &programOk);
        if (!programOk) {
            GLint logLength;
            char* log;

            glGetProgramiv(name, GL_INFO_LOG_LENGTH, &logLength);
            log = static_cast<char*>(malloc(logLength));
            glGetProgramInfoLog(name, logLength, NULL, log);
            
            std::string str(log); 
            free(log);

            throw std::runtime_error(str);
        }
    }
}

Program::~Program() {
    glDeleteProgram(name);
}

GLint Program::getName() const {
    return name;
}

void Program::bind() const {
    glUseProgram(name);
}

GLint Program::getUniformLocation(char const* identifier) const {
    return glGetUniformLocation(name, identifier);
}

GLint Program::getAttribLocation(char const* identifier) const {
    return glGetAttribLocation(name, identifier);
}

void Program::setUniform(GLint location, GLint value) const {
    glUniform1i(location, value);
}

void Program::setUniform(GLint location, GLfloat value) const {
    glUniform1f(location, value);
}

void Program::setUniform(GLint location, vec2 const& value) const {
    glUniform2f(location, value.x, value.y);
}

void Program::setUniform(GLint location, vec3 const& value) const {
    glUniform3f(location, value.x, value.y, value.z);
}

void Program::setUniform(GLint location, vec4 const& value) const {
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Program::setUniform(GLint location, mat2 const& value) const {
    glUniformMatrix2fv(location, 1, 0, glm::value_ptr(value));
}

void Program::setUniform(GLint location, mat3 const& value) const {
    glUniformMatrix3fv(location, 1, 0, glm::value_ptr(value));
}

void Program::setUniform(GLint location, mat4 const& value) const {
    glUniformMatrix4fv(location, 1, 0, glm::value_ptr(value));
}

} // namespace game
