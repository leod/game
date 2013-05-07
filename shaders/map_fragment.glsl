#version 330

uniform sampler2D vision;
uniform sampler2D diffuse;

smooth in vec2 frag_texCoord;
smooth in vec2 frag_screenCoord;
smooth in float frag_alpha;

out vec4 fragColor;

void main() {
    float alpha = clamp(texture(vision, frag_screenCoord).r + 0.3, 0, 1);
    alpha *= frag_alpha;

    //fragColor = texture(diffuse, frag_texCoord) * alpha;
    fragColor = vec4(1, 0, 1, 1) * alpha;
}

