#version 450
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

layout(std140, binding = 1) uniform Camera {
    mat4 view;
    mat4 projection;
    
    vec3 viewPos;
    vec3 cameraFront;
};

layout(std140, binding = 2) uniform Light {
    vec3 lightDirection;

    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
};

struct Material {
    sampler2D diffuse;

    float ambientStrength;
    float specularStrength;
    float shininess;
};

out vec4 FragColor;

uniform Material material;

void main() {
    vec3 objectColor = texture(material.diffuse, TexCoord).xyz;   

    // ambient
    vec3 ambient = material.ambientStrength * lightAmbient; 

    // diffuse
    float diff = max(dot(Normal, lightDirection), 0.0f);    
    vec3 diffuse = lightDiffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDirection, Normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightSpecular * (material.specularStrength * spec);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
