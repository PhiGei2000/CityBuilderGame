#pragma once

struct MouseMoveEvent {
    float x, y;

    float lastX, lastY;
};

struct MouseButtonEvent {
    float x, y;

    int button, action, mods;
};
