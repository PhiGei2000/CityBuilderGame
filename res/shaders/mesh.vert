#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
    vec3 tangentLightDirection;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

layout(std140, binding = 1) uniform Camera {
    mat4 view;
    mat4 projection;

    vec3 viewPos;
    vec3 cameraTarget;
};

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
    vec4 position = vec4(aPos, 1.0);

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

    vs_out.TBN = mat3(T, B, N);

    gl_Position = projection * view * model * position;
    vs_out.FragPos = vec3(model * position);

    vs_out.TexCoord = aTexCoord;
    vs_out.tangentLightDirection = normalize(vs_out.TBN * lightDirection);
    vs_out.tangentViewPos = vs_out.TBN * viewPos;
    vs_out.tangentFragPos = vs_out.TBN * vs_out.FragPos;
}