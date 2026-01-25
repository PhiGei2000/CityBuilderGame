#pragma once
#include <tuple>

#include <glm/glm.hpp>

struct Rectangle {
    float x, y, width, height;

    inline Rectangle() {
    }

    inline Rectangle(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {
    }

    inline Rectangle(const glm::vec2& pos, const glm::vec2& size)
        : x(pos.x), y(pos.y), width(size.x), height(size.y) {
    }

    inline bool pointInside(float x, float y) const {
        return x >= this->x && x <= this->x + this->width &&
               y >= this->y && y <= this->y + this->height;
    }

    inline glm::vec2 getPosition() const {
        return glm::vec2{x, y};
    }

    inline std::tuple<float, float> getX() const {
        return {x, x + width};
    }

    inline std::tuple<float, float> getY() const {
        return {y, y + height};
    }
};
