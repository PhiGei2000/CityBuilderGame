#version 450
layout(points) in;
layout(line_strip, max_vertices=2) out;

layout(std140, binding = 1) uniform Camera {
    mat4 view;
    mat4 projection;

    vec3 viewPos;
    vec3 cameraTarget;
};

in VS_OUT {
    vec3 pos;
    vec3 normal;
} gs_in[];

void main() {
    gl_Position = projection * view * vec4(gs_in[0].pos, 1.0);
    EmitVertex();    

    gl_Position = projection * view * vec4(gs_in[0].pos + gs_in[0].normal, 1.0);
    EmitVertex();
    EndPrimitive();
}