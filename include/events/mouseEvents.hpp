#pragma once
#include "event.hpp"

struct MouseMoveEvent : public Event<false> {
    float x, y;

    float lastX, lastY;

    inline MouseMoveEvent(float x, float y, float lastX, float lastY)
        : x(x), y(y), lastX(lastX), lastY(lastY) {
    }
};

struct MouseButtonEvent : public Event<false> {
    float x, y;

    int button, action, mods;

    inline MouseButtonEvent(float x, float y, int button, int action, int mods)
        : x(x), y(y), button(button), action(action), mods(mods) {
    }
};

struct MouseScrollEvent : public Event<false> {
    float xoffset, yoffset;

    inline MouseScrollEvent(float xoffset, float yoffset)
        : xoffset(xoffset), yoffset(yoffset) {
    }
};
