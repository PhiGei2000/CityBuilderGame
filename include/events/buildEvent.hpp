#pragma once
#include "misc/buildingType.hpp"

#include <glm/glm.hpp>

enum class BuildType
{
    DEFAULT,
    BEGIN,
    END
};

struct BuildEvent {
    glm::ivec2 gridPosition;
    BuildingType type;

    BuildType buildType;
};
