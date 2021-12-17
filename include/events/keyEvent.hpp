#pragma once
#include "event.hpp"

namespace trafficSimulation::events {
    struct KeyEvent : public Event{
        int key;
        int scancode;
        int action;
        int mods;
    };
} // namespace trafficSimulation::events