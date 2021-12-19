#version 330 
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat3 transform;

out vec2 texCoord;

void main() {
    vec3 position = transform * vec3(aPos.x, aPos.y, 1.0);
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);

    texCoord = aTexCoord;
}