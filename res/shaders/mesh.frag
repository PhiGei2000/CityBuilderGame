#version 450
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

struct Light {
    vec3 direction;
    vec3 color;
};

out vec4 FragColor;

uniform Light light;
uniform vec3 viewPos;
uniform sampler2D diffuse;

void main() {
    vec4 objectColor = texture(diffuse, TexCoord);

    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * light.color;

    float diff = max(dot(Normal, light.direction), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-light.direction, Normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
    vec3 specular = specularStrength * spec * light.color;

    vec3 result = (ambient + diffuse + specular) * objectColor.xyz;
    FragColor = vec4(result, objectColor.w);
}