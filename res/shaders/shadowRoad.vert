#version 450
// vertex attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
// road tile attributes
layout(location = 5) in vec2 gridPos; // position inside the current chunk
layout(location = 6) in int rotation;

layout(std140, binding = 2) uniform Light {
    mat4 lightView[cascadeCount];
    mat4 lightProjection[cascadeCount];

    vec3 lightDirection;

    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;

    float cascadeFarPlanes[cascadeCount];
};

uniform mat4 model;

void main() {
    float angle = radians(rotation * 90);
    float angleSin = sin(angle);
    float angleCos = cos(angle);

    mat3 transform = mat4(
        vec4(angleCos, 0.0, angleSin, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(-angleSin, 0.0, angleCos, 0.0),
        vec4(gridPos.x, 0.0, gridPos.y, 1.0));

    gl_Position = model * transform * vec4(aPos + 0.1 * lightDirection, 1.0);
}
