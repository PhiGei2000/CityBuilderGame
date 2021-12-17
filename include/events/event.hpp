#pragma once

#include "application.hpp"

namespace trafficSimulation::events {
    struct Event {
        Application* application;
    };
}