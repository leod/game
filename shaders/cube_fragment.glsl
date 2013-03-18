#version 330

uniform sampler2D vision;

smooth in vec4 color;
smooth in vec2 coord;

out vec4 fragColor;

void main() {
    float alpha = clamp(texture(vision, vec2(1, 0)).r + 0.3, 0, 1);
    fragColor = color * alpha;
}

