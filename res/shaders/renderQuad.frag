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

// border
uniform float borderThickness;
uniform vec4 borderColor;

// widget area
uniform Rectangle widgetArea;

out vec4 FragColor;

float distanceFromEdge();

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
        vec2 d = min(u, v);
        if (d.x < 0 && d.y < 0 && length(d) > cornerRadius) {
            discard;
        }
    }

    if (borderThickness > 0 && distanceFromEdge() < borderThickness) {
        FragColor = borderColor;
        return;
    }

    // render text and texture
    if (text) {
        float val = texture(tex, texCoord).r;
        vec4 sampled = vec4(val, val, val, val);
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

float distanceFromEdge() {
    float d_minX = fragPosition.x - widgetArea.position.x;
    float d_maxX = widgetArea.size.x - d_minX;

    float d_minY = fragPosition.y - widgetArea.position.y;
    float d_maxY = widgetArea.size.y - d_minY;

    return min(min(d_minX, d_maxX), min(d_minY, d_maxY));
}