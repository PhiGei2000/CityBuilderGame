#pragma once

#include "entityEvent.hpp"

struct CameraUpdateEvent : public EntityEvent<true> {
    bool sizeUpdated;
    bool positionUpdated;
    bool rotationUpdated;

    inline CameraUpdateEvent(entt::entity entity, bool sizeUpdated, bool positionUpdated, bool rotationUpdated)
        : EntityEvent<true>(entity), sizeUpdated(sizeUpdated), positionUpdated(positionUpdated), rotationUpdated(rotationUpdated) {
    }
};
