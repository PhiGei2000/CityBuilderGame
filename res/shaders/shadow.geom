#version 450
layout(triangles, invocations = cascadeCount) in;
layout(triangle_strip, max_vertices = 3) out;

layout(std140, binding = 2) uniform Light {
    mat4 lightView[cascadeCount];
    mat4 lightProjection[cascadeCount];

    vec3 lightDirection;

    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;

    float cascadeFarPlanes[cascadeCount];
};

void main() {
    gl_Layer = gl_InvocationID;
    for (int i = 0; i < 3; i++) {
        gl_Position = lightProjection[gl_InvocationID] * lightView[gl_InvocationID] * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
