#pragma once
#include "misc/roads/roadGraph.hpp"
#include "misc/roads/roadTile.hpp"

#include <glm/gtx/hash.hpp>
#include <map>
#include <unordered_map>

struct RoadComponent {
    std::unordered_map<glm::ivec2, RoadTile> tiles;

    RoadGraph graph;
};
