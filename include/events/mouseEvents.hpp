#pragma once
#include "event.hpp"

struct MouseMoveEvent : public Event {
    float x, y;

    float lastX, lastY;
};

struct MouseButtonEvent : public Event {
    float x, y;

    int button, action, mods;
};
