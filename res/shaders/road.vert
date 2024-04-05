#version 450
// vertex attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
// road tile attributes
layout(location = 5) in vec3 gridPos; // position inside the current chunk
layout(location = 6) in int rotation;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
    vec3 tangentLightDirection;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
}
vs_out;

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
    float angle = radians(rotation * 90);
    float angleSin = sin(angle);
    float angleCos = cos(angle);

    mat3 transform = mat4(
        vec4(angleCos, 0.0, angleSin, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(-angleSin, 0.0, angleCos, 0.0),
        vec4(gridPos, 1.0));

    vec4 position = transform * vec4(aPos, 1.0);

    // calculate TBN matrix to transform world vectors into tangent space
    mat3 normalMatrix = transpose(inverse(mat3(model * transform)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);

    vs_out.TBN = transpose(mat3(T, B, N));

    gl_Position = projection * view * model * position;
    vs_out.FragPos = vec3(model * position);
    vs_out.TexCoord = aTexCoord;

    // transform light direction vector to tangent space
    vs_out.tangentLightDirection = normalize(vs_out.TBN * lightDirection);
    // transform view and fragment position to tangent space
    vs_out.tangentViewPos = vs_out.TBN * viewPos;
    vs_out.tangentFragPos = vs_out.TBN * vs_out.FragPos;
}
