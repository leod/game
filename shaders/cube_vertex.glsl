#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform vec3 diffuse;

smooth out vec4 color;

void main() {
    vec3 normalWorldSpace = normalize(mat3(model) * normal);
    float cosAngle = clamp(dot(normalWorldSpace, vec3(1, 0, 0)) + 0.9, 0, 1);
    color = vec4(cosAngle * diffuse, 1.0);

    gl_Position = projection *
                  view *
                  model *
                  vec4(position.x, position.y, position.z, 1);
}

