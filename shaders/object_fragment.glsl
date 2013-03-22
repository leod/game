#version 330

uniform sampler2D vision;

smooth in vec4 color;
smooth in vec2 coord;

out vec4 fragColor;

void main() {
    float alpha = texture(vision, coord).r;

    if (alpha < 0.01)
        discard;

    fragColor = color * alpha;
}

