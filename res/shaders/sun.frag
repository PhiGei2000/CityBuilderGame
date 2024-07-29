#version 450
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
}
fs_in;

out vec4 fragColor;

uniform sampler2D sunDiffuse;

void main() {
    // fragColor = texture(sunDiffuse, fs_in.TexCoord);
    fragColor = vec4(1.0, 0.92, 0.36, 1.0);
}