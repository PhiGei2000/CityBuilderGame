#pragma once
#include "misc/direction.hpp"

#include <set>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

struct RoadGraphEdge {
    glm::ivec2 start, end;
    int length;

    RoadGraphEdge();
    RoadGraphEdge(const glm::ivec2& start, const glm::ivec2& end);    

    bool contains(const glm::ivec2& position) const;
};

struct RoadGraphNode {
    glm::ivec2 position;
    std::array<RoadGraphEdge, 4> edges;

    RoadGraphNode();
    RoadGraphNode(const glm::ivec2& position);
};

struct RoadGraph {
    std::unordered_map<glm::ivec2, RoadGraphNode> nodes;

    void insertNode(const glm::ivec2& position);

    void updateNodeConnection(const glm::ivec2& position, Direction dir);
};
