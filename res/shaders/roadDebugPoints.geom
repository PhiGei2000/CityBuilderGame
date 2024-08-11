#version 450
layout(lines) in;
layout(points, max_vertices=1) out;

layout(std140, binding=1) uniform Camera {
    mat4 view;
    mat4 projection;

    vec3 viewPos;
    vec3 cameraTarget;
};

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * gl_in[1].gl_Position;
    gl_PointSize = 4.0;
    EmitVertex();
    EndPrimitive();
}
