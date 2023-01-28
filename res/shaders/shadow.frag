#version 450

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

void main() {     
    // discard fragments if they are transparent
    if (material.dissolve < 1) {
        discard;
    }    
}