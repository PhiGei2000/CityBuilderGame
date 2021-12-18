#pragma once
#include "event.hpp"

    struct MouseMoveEvent : public Event {
        float x, y;

        float lastX, lastY;
    };