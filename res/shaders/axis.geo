#version 330
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraTarget;

out vec4 AxisColor;

void main() {
    AxisColor = gl_in[0].gl_Position;

    gl_Position = projection * view * (vec4(cameraTarget, 0.0) + gl_in[0].gl_Position);
    EmitVertex();

    gl_Position = projection * view * vec4(vec3(cameraTarget), 1.0);
    EmitVertex();
}

