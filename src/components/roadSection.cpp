#include "misc/roads/roadSection.hpp"

RoadSection::RoadSection(const glm::ivec2& start, const glm::ivec2& end, RoadType type, int rotation)
    : start(start), end(end), type(type), rotation(rotation) {
}
