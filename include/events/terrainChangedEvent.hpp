#pragma once

#include "entityEvent.hpp"

struct TerrainChangedEvent : public EntityEvent {
    bool regenerateMesh = false;
};
