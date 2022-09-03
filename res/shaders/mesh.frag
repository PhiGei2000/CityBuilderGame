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

    float ambientStrength = 0.75f;    

    float diffuse = max(dot(Normal, light.direction), 0.0f);    

    // float specularStrength = 0.5f;
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(light.direction, Normal);

    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // float specular = specularStrength * spec;

    vec3 result = (ambientStrength + diffuse /*+ specular*/) * objectColor.xyz * light.color;
    FragColor = vec4(result, 1.0);
}