#version 450
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 projection;
uniform bool flipV;

out vec2 texCoord;

void main() {
    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);

    if (flipV) {
        texCoord = vec2(aTexCoord.x, 1 - aTexCoord.y);
    }
    else {
        texCoord = aTexCoord;
    }
}