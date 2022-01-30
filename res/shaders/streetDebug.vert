#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

layout(std140, binding = 1) uniform Matrices {
    mat4 view;
    mat4 projection;
};

out vec4 color;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    color = aColor;
}
