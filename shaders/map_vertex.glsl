#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

smooth out vec2 frag_texCoord;
smooth out vec2 frag_screenCoord;
smooth out float frag_alpha;

void main() {
    gl_Position = projection * view * model *
                  vec4(position.x, position.y, position.z, 1);
    frag_texCoord = texCoord;
    frag_screenCoord = (gl_Position.xy / gl_Position.w + 1) / 2;

    vec3 normalWorldSpace = normalize(mat3(model) * normal);
    float cosAngle = clamp(dot(normalWorldSpace, normalize(vec3(1, -1, -0.3))),
                           0, 1);

    frag_alpha = clamp(cosAngle + 0.5, 0, 1);
}

