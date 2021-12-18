#version 330
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D diffuse;

void main() {
    FragColor = texture(diffuse, TexCoord);
}