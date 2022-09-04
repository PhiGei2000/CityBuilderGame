#pragma once

#include "entityEvent.hpp"

struct CameraUpdateEvent : public EntityEvent {
    bool sizeUpdated;
    bool positionUpdated;
    bool rotationUpdated;
};
