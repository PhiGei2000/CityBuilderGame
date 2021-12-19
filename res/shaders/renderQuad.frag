#version 330
in vec2 texCoord;

uniform vec4 color;
// uniform sampler2D texture;

out vec4 FragColor;

void main() {
    FragColor = color;
}