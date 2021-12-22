#version 330
in vec2 texCoord;

uniform vec4 color;
uniform sampler2D tex;

uniform bool useTexture;
uniform bool text;

out vec4 FragColor;

void main() {
    if (text) {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r);
        FragColor = color * sampled;
    }
    else if (useTexture) {
        FragColor = texture(tex, texCoord);
    }
    else {
        FragColor = color;
    }
}