#pragma once
#include "misc/buildingType.hpp"

#include <glm/glm.hpp>

struct BuildEvent {
    glm::ivec2 gridPosition;
    BuildingType type;
};
