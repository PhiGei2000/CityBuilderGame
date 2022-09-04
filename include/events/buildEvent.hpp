#pragma once
#include "misc/buildingType.hpp"

#include <glm/glm.hpp>

enum class BuildAction
{
    DEFAULT,
    BEGIN,
    END
};

struct BuildEvent {
    glm::ivec2 gridPosition;
    BuildingType type;

    BuildAction action;
};
