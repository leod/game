#version 330

uniform sampler2D vision;

smooth in vec4 color;
smooth in vec2 coord;

out vec4 fragColor;

float smple(vec2 c) {
    return (texture(vision, coord + vec2(-0.001 + 0.0001, -0.001)).r +
            texture(vision, coord + vec2(0.001, -0.001 + 0.001)).r +
            texture(vision, coord + vec2(0.001, 0.001)).r +
            texture(vision, coord + vec2(-0.001, 0.001 - 0.0001)).r +
            texture(vision, coord + vec2(-0.0005, -0.0005 + 0.0001)).r +
            texture(vision, coord + vec2(0.0005 + 0.0001, -0.0005)).r +
            texture(vision, coord + vec2(0.0005, 0.0005 - 0.0001)).r +
            texture(vision, coord + vec2(-0.0005 - 0.0001, 0.0005)).r) / 8.0f;
}

void main() {
    float alpha = clamp(texture(vision, coord).r + 0.5, 0, 1);
    fragColor = color * alpha;
}

