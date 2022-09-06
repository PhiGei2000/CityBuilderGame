#version 450
layout(points) in;
layout(line_strip, max_vertices = 2) out;

layout(std140, binding = 1) uniform Camera {
    mat4 view;          // 64 bytes
    mat4 projection;    // 64 bytes

    vec3 viewPos;       // 16 bytes
    vec3 cameraTarget;  // 16 bytes
};

out vec4 AxisColor;

void main() {
    AxisColor = gl_in[0].gl_Position;

    gl_Position = projection * view * (vec4(cameraTarget, 0.0) + gl_in[0].gl_Position);
    EmitVertex();

    gl_Position = projection * view * vec4(cameraTarget, 1.0);
    EmitVertex();
}
