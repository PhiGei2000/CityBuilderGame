#pragma once
#include "misc/roads/roadSection.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

struct RoadComponent {
    std::unordered_map<glm::ivec2, RoadSection> sections;    
};
