#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
}
vs_out;

layout(std140, binding = 1) uniform Camera {
    mat4 view;
    mat4 projection;

    vec3 viewPos;
    vec3 cameraTarget;
};

uniform mat4 model;

void main() {
    vec4 position = vec4(aPos, 1.0);

    gl_Position = projection * view * model * position;
    vs_out.FragPos = vec3(model * position);
    vs_out.TexCoord = aTexCoord;
}