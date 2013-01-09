#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 diffuse;

smooth out vec4 color;

void main() {
    vec3 normalWorldSpace = normalize(mat3(model) * normal);
    float cosAngle = clamp(dot(normalWorldSpace, normalize(vec3(1, -1, -0.3))), 0, 1);
    color = vec4(clamp(cosAngle + 0.5, 0, 1) * diffuse, 1.0);

    gl_Position = projection *
                  view *
                  model *
                  vec4(position.x, position.y, position.z, 1);
}

