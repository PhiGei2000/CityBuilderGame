#version 450
in vec2 texCoord;
in vec2 fragPosition;

struct Rectangle {
    vec2 position;
    vec2 size;
};

// color and texture
uniform vec4 color;
uniform sampler2D tex;

//
uniform bool useTexture;
uniform bool text;
uniform float cornerRadius;

// widget area
uniform Rectangle widgetArea;

out vec4 FragColor;

void main() {
    // render round corners if radius is greather than zero
    if (cornerRadius > 0) {
        // calculate vector pointing from the current frag to the nearest inner edge
        vec2 r = vec2(cornerRadius);
        // left bottom corner to frag pos
        vec2 u = fragPosition - (widgetArea.position + r);
        // frag pos to top right corner
        vec2 v = widgetArea.position + widgetArea.size - r - fragPosition;

        // minimum distance vector
        vec2 d = min(u,v);        
        if (d.x < 0 && d.y < 0 && length(d) > cornerRadius) {
            discard;
        }
    }    

    // render text and texture
    if (text) {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r);
        FragColor = color * sampled;
    }
    // render texture
    else if (useTexture) {
        FragColor = texture(tex, texCoord);
    }
    // render color
    else {
        FragColor = color;
    }
}