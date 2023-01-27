#version 450
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2DArray shadowMaps;
uniform int layerId;

void main() {
    float depth = texture(shadowMaps, vec3(texCoord.x, 1-texCoord.y, layerId)).r;

    FragColor = vec4(vec3(depth), 1.0);
}
