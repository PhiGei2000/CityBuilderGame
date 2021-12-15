#pragma once

namespace trafficSimulation::events {
    struct KeyEvent {
        int key;
        int scancode;
        int action;
        int mods;
    };
} // namespace trafficSimulation::events