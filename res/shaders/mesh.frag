#version 450
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

struct Light {
    vec3 direction;
    vec3 color;
};

struct Material {
    sampler2D diffuse;

    float ambientStrength;
    float specularStrength;
    float shininess;
};

out vec4 FragColor;

uniform Light light;
uniform vec3 viewPos;
uniform Material material;

void main() {
    vec4 objectColor = texture(material.diffuse, TexCoord);    

    float diffuse = max(dot(Normal, light.direction), 0.0f);    
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(light.direction, Normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float specular = material.specularStrength * spec;

    vec3 result = (material.ambientStrength + diffuse + specular) * objectColor.xyz * light.color;
    FragColor = vec4(result, 1.0);
}
