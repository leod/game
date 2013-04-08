#version 330

uniform sampler2D vision;
uniform sampler2D floor;

smooth in vec2 frag_texCoord;
smooth in vec2 frag_screenCoord;

out vec4 frag_out;

void main() {
    vec4 diffuse = texture(floor, frag_texCoord);
    float alpha = clamp(texture(vision, frag_screenCoord).r + 0.3, 0, 1);
    frag_out = diffuse * alpha;
}

