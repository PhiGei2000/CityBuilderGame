#version 450
layout (location = 0) in vec3 axis;

void main() {
    gl_Position = vec4(axis, 1.0);
}

