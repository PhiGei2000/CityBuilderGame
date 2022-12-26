#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(std140, binding = 2) uniform Light {
    mat4 lightView;
    mat4 lightProjection;

    vec3 lightDirection;

    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
};

uniform mat4 model;

void main() {
    gl_Position = lightProjection * lightView * model * vec4(aPos, 1.0);
}