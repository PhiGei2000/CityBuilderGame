#pragma once
#include "component.hpp"

enum class DebugMode : unsigned int {
    OFF = 0x00,
    AXIS = 0x01,
    SHADOW_MAPS = 0x02,
    CAR_PATHS = 0x04,
};

struct DebugComponent : public Component<false> {
    DebugMode mode = DebugMode::OFF;
};
