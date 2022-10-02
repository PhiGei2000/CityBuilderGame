#pragma once
#include "component.hpp"
#include "misc/roads/roadGraph.hpp"

#include <glm/gtx/hash.hpp>
#include <map>
#include <unordered_map>

struct RoadComponent : public Component {
    RoadGraph graph;    
};
