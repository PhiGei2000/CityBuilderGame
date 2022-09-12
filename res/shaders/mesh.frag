#version 450
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

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
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
    float specularStrength;
};

out vec4 FragColor;

uniform Material material;

void main() {
    vec3 diffuseColor = texture(material.diffuse, TexCoord).rgb;
    vec3 specularColor = texture(material.specular, TexCoord).rgb;

    // ambient
    vec3 ambient = lightAmbient * diffuseColor;

    // diffuse
    float diff = max(dot(Normal, -lightDirection), 0.0f);
    vec3 diffuse = lightDiffuse * (diff * diffuseColor);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(viewDir - lightDirection);

    float spec = pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = material.specularStrength * lightSpecular * spec;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
