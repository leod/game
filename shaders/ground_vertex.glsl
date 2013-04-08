#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

smooth out vec2 frag_texCoord;
smooth out vec2 frag_screenCoord;

void main() {
    gl_Position = projection * view * model *
                  vec4(position.x, position.y, position.z, 1);
    frag_texCoord = texCoord;
    frag_screenCoord = (gl_Position.xy / gl_Position.w + 1) / 2;
}

