#pragma once

#include "configuration.hpp"

// holds data to describe street geometry
struct StreetGeometryCrossSection {
    float laneWidth;
    float laneHeight;

    float pathwayWidth;
    float pathwayHeight;

    inline StreetGeometryCrossSection(float laneWidth, float laneHeight, float pathwayHeight)
        : laneWidth{laneWidth}, laneHeight{laneHeight}, pathwayHeight{pathwayHeight} {
            pathwayWidth = (static_cast<float>(Configuration::gridSize) - laneWidth) / 2.0f;
    }
};
