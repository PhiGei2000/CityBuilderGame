#version 450
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
    vec3 tangentLightDirection;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
}
fs_in;

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

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;

    sampler2D ambientTexture;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D normalMap;

    float shininess;
    float specularStrength;
    float dissolve;

    int illuminationModel;

    bool useAmbientTexture;
    bool useDiffuseTexture;
    bool useSpecularTexture;
};

out vec4 FragColor;

uniform Material material;
uniform sampler2DArray shadowMaps;
uniform bool preview;

vec3 calcAmbientLight(vec3 ambientColor);
vec3 calcDiffuseLight(vec3 normal, vec3 diffuseColor);
vec3 calcSpecularLight(vec3 normal, vec3 specularColor);
float shadowCalculation(vec3 normal);

// float biasValues[cascadeCount] = float[](0.05, 0.005, 0.005, 0.001);

void main() {
    // init colors
    vec3 ambientColor = material.ambientColor;
    if (material.useAmbientTexture) {
        ambientColor *= texture(material.ambientTexture, fs_in.TexCoord).rgb;
    }

    vec3 diffuseColor = material.diffuseColor;
    if (material.useDiffuseTexture) {
        diffuseColor *= texture(material.diffuseTexture, fs_in.TexCoord).rgb;
    }

    vec3 specularColor = material.specularColor;
    if (material.useSpecularTexture) {
        specularColor *= texture(material.specularTexture, fs_in.TexCoord).rgb;
    }

    // extract the normal vector from the normal map. The normal vector is then in tangent space
    vec3 normal = texture(material.normalMap, fs_in.TexCoord).rgb;
    normal = normal * 2.0 - 1.0;
    // normal = normalize(transpose(fs_in.TBN) * normal);

    float shadow = shadowCalculation(normal);

    vec3 ambient, diffuse, specular;
    switch (material.illuminationModel) {
        case 0:
            FragColor = vec4((1.0 - shadow) * diffuseColor, material.dissolve);
            break;
        case 1:
            ambient = calcAmbientLight(ambientColor);
            diffuse = calcDiffuseLight(normal, diffuseColor);
            FragColor = vec4(ambient + (1.0 - shadow) * diffuse, material.dissolve);
            break;
        case 2:
            ambient = calcAmbientLight(ambientColor);
            diffuse = calcDiffuseLight(normal, diffuseColor);
            specular = calcSpecularLight(normal, specularColor);
            FragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), material.dissolve);
    }

    if (preview) {
        FragColor *= vec4(0.0, 0.6, 0.6, 1.0);
    }
}

vec3 calcAmbientLight(vec3 ambientColor) {
    return lightAmbient * ambientColor;
}

vec3 calcDiffuseLight(vec3 normal, vec3 diffuseColor) {
    float diff = max(dot(normal, fs_in.tangentLightDirection), 0.0);
    return diff * lightDiffuse * diffuseColor;
}

vec3 calcSpecularLight(vec3 normal, vec3 specularColor) {
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec3 halfwayDir = normalize(viewDir + fs_in.tangentLightDirection);
    float dotVal = max(dot(normal, halfwayDir), 1E-6);

    float spec = pow(dotVal, material.shininess);
    return spec * lightSpecular * specularColor;
}

float shadowCalculation(vec3 normal) {
    // transform fragment position from world space into view space
    vec4 fragPosViewSpace = view * vec4(fs_in.FragPos, 1.0);

    // find the corresponding shadow map
    int mapIndex = cascadeCount - 1;
    for (int i = 0; i < cascadeCount; i++) {
        if (abs(fragPosViewSpace.z) < cascadeFarPlanes[i]) {
            mapIndex = i;
            break;
        }
    }

    // transform the fragment position from world space into light space and extract depth from the shadow map
    vec4 fragPosLightSpace = lightProjection[mapIndex] * lightView[mapIndex] * vec4(fs_in.FragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

    // if depth greater than one render no shadow
    if (currentDepth > 1.0 || projCoords.z > 1.0) {
        return 0.0;
    }

    // calculate bias and apply pcf
    float shadowBias[] = {2E-3, 1.5E-3, 1.5E-3, 0.0001};
    float maxShadowBias[] = {2E-4, 1.5E-4, 1.5E-4, 0.0005};
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps, 0));
    float cosTheta = dot(normal, fs_in.tangentLightDirection);
    float bias = max(abs(shadowBias[mapIndex] * (1 - cosTheta)), maxShadowBias[mapIndex]);

    float shadow = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float closestDepth = texture(shadowMaps, vec3(projCoords.xy + vec2(x, y) * texelSize, mapIndex)).r;
            shadow += (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    return shadow;
}
