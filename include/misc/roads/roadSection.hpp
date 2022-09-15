#pragma once
#include "roadTypes.hpp"

#include <glm/glm.hpp>

struct RoadSection {
    glm::ivec2 start;
    glm::ivec2 end;

    RoadType type;
};
