#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

#define splitCount 2

out vec3 FragPos;
out vec2 TexCoord;
out mat3 TBN;
out vec4 FragPosLightSpace[splitCount];
out float ClipSpacePosZ;

layout(std140, binding = 1) uniform Camera {
    mat4 view;
    mat4 projection;

    vec3 viewPos;
    vec3 cameraTarget;
};

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

void main() {
    vec4 position = vec4(aPos, 1.0);

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

    TBN = mat3(T, B, N);

    gl_Position = projection * view * model * position;
    FragPos = vec3(model * position);
    for (int i = 0; i < splitCount; i++) {
        FragPosLightSpace[i] = lightProjection[i] * lightView[i] * vec4(FragPos, 1.0);
    }

    TexCoord = aTexCoord;
}