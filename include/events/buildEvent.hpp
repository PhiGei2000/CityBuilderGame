#pragma once
#include "misc/buildingType.hpp"

#include <glm/glm.hpp>

enum class BuildAction {
    DEFAULT,
    BEGIN,
    END,
    SELECT,
    PREVIEW
};

enum class BuildShape {
    POINT,
    LINE,
    AREA
};

struct BuildEvent {
    glm::ivec2 gridPosition;
    BuildingType type;

    BuildAction action;
    glm::ivec2 buildingStartPosition = glm::ivec2(-1);
    BuildShape shape = BuildShape::LINE;
    bool xFirst = false;
};
