#pragma once
#include "misc/buildingType.hpp"

#include <glm/glm.hpp>

enum class BuildAction {
    DEFAULT,
    BEGIN,
    END,
    SELECT
};

struct BuildEvent {
    glm::ivec2 gridPosition;
    BuildingType type;

    BuildAction action;
    glm::ivec2 buildingStartPosition = glm::ivec2(-1);
};
