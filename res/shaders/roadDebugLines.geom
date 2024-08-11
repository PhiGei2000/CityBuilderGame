#version 450
layout(lines) in;
layout(line_strip, max_vertices = 3) out;

layout(std140, binding = 1) uniform Camera {
    mat4 view;          // 64 bytes
    mat4 projection;    // 64 bytes

    vec3 viewPos;       // 16 bytes
    vec3 cameraTarget;  // 16 bytes
};

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = projection * view * model * gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();
}