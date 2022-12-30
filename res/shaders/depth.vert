#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

#define splitCount 2

layout(std140, binding = 2) uniform Light {
    mat4 lightView[splitCount];
    mat4 lightProjection[splitCount];

    vec3 lightDirection;

    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;

    float cascadeFarPlanes[splitCount];
};

uniform mat4 model;
uniform int mapIndex;

void main() {
    gl_Position = lightProjection[mapIndex] * lightView[mapIndex] * model * vec4(aPos, 1.0);
}