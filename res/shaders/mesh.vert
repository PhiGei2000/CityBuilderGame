#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

layout(std140, binding = 1) uniform Matrices {
    mat4 view;
    mat4 projection;
};

uniform mat4 model;

void main() {
    vec4 position = vec4(aPos, 1.0);

    gl_Position = projection * view * model * position;
    FragPos = vec3(model * position);

    TexCoord = aTexCoord;
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
}