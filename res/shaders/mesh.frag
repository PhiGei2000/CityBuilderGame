#version 450
in vec3 FragPos;
in vec2 TexCoord;
in mat3 TBN;

layout(std140, binding = 1) uniform Camera {
    mat4 view;
    mat4 projection;

    vec3 viewPos;
    vec3 cameraTarget;
};

layout(std140, binding = 2) uniform Light {
    vec3 lightDirection;

    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
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

vec3 calcAmbientLight(vec3 ambientColor);
vec3 calcDiffuseLight(vec3 normal, vec3 diffuseColor);
vec3 calcSpecularLight(vec3 normal, vec3 specularColor);

void main() {
    // init colors
    vec3 ambientColor = material.ambientColor;
    if (material.useAmbientTexture) {
        ambientColor = ambientColor * texture(material.ambientTexture, TexCoord).rgb;
    }

    vec3 diffuseColor = material.diffuseColor;
    if (material.useDiffuseTexture) {
        diffuseColor = texture(material.diffuseTexture, TexCoord).rgb;
    }

    vec3 specularColor = material.specularColor;
    if (material.useSpecularTexture) {
        specularColor = texture(material.specularTexture, TexCoord).rgb;
    }

    vec3 normal = texture(material.normalMap, TexCoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);

    vec3 ambient, diffuse, specular;
    switch (material.illuminationModel) {
    case 0:
        FragColor = vec4(diffuseColor, material.dissolve);
        break;
    case 1:
        ambient = calcAmbientLight(ambientColor);
        diffuse = calcDiffuseLight(normal, diffuseColor);
        FragColor = vec4(ambient + diffuse, material.dissolve);
        break;
    case 2:
        ambient = calcAmbientLight(ambientColor);
        diffuse = calcDiffuseLight(normal, diffuseColor);
        specular = calcSpecularLight(normal, specularColor);
        FragColor = vec4(ambient + diffuse + specular, material.dissolve);
    }
}

vec3 calcAmbientLight(vec3 ambientColor) {
    return lightAmbient * ambientColor;
}

vec3 calcDiffuseLight(vec3 normal, vec3 diffuseColor) {
    float diff = max(dot(normal, -lightDirection), 0.0);
    return diff * lightDiffuse * diffuseColor;
}

vec3 calcSpecularLight(vec3 normal, vec3 specularColor) {
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(viewDir - lightDirection);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    return spec * lightSpecular * specularColor;
}
