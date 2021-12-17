#pragma once
#include "event.hpp"

namespace trafficSimulation::events {
    struct FramebufferSizeEvent : public Event {
        int width;
        int height;
    };
} // namespace trafficSimulation::events