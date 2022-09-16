#pragma once
#include "roadTypes.hpp"

#include <glm/glm.hpp>

struct RoadSection {
    glm::ivec2 start;
    glm::ivec2 end;

    RoadType type;
    int rotation = 0;

    RoadSection(const glm::ivec2& start, const glm::ivec2& end, RoadType type = RoadType::EDGE, int rotation = 0);
};
