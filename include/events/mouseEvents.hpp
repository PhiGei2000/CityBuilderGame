#pragma once
#include "event.hpp"

namespace trafficSimulation::events {
    struct MouseMoveEvent : public Event {
        float x, y;

        float lastX, lastY;
    };
} // namespace trafficSimulation::events