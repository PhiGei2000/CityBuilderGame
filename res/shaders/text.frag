#version 330
in vec2 texCoord;

uniform sampler2D text;
uniform vec3 textColor;

out vec4 fragColor;

void main() {
    vec2 flippedTexCoord = vec2(texCoord.x, 1 - texCoord.y);

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, flippedTexCoord).r);
    fragColor = vec4(textColor, 1.0) * sampled;
}