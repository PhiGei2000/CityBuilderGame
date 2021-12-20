#pragma once

struct Rectangle {
    float x, y, width, height;

    inline bool pointInside(float x, float y) const {
        return x >= this->x && x <= this->x + this->width &&
               y >= this->y && y <= this->y + this->height;
    }
};
