#pragma once
#include "misc/roads/roadSection.hpp"

#include <unordered_map>
#include <map>

struct RoadComponent {
    std::unordered_map<glm::ivec2, unsigned int> keys;
    std::map<unsigned int, RoadSection> sections;    
};
