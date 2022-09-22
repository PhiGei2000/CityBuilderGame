#pragma once
#include "misc/direction.hpp"
#include "misc/roads/roadTypes.hpp"

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

    bool connected(Direction dir) const;

    /// @brief Determines how often the road tile has to rotated by 90 degrees to fit the connected roads.
    /// @return A value beween 0 and 4 that specifies how often the road tile has to be rotated.
    int getRotation() const;

    /// @brief Determines the road type based on the connections
    /// @return The determined road type
    RoadType getType() const;
};

struct RoadGraph {  
    std::unordered_map<glm::ivec2, RoadGraphNode> nodes;

    void insertNode(const glm::ivec2& position);

    void updateNodeConnection(const glm::ivec2& position, Direction dir);  

    void clear();  
};
