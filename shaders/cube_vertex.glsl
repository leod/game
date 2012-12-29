#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 modelview;

void main() {
    gl_Position = projection *
                  modelview *
                  vec4(position.x, position.y, position.z, 1);
}

