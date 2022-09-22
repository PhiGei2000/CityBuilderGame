#pragma once
#include "misc/direction.hpp"
#include "misc/roads/roadTypes.hpp"

#include <array>
#include <unordered_map>
#include <queue>

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

    /// @brief Checks if the node has a connection in the specified direction.
    /// @param dir The direction to check
    /// @return True if the node is conntected, false otherwise
    bool connected(Direction dir) const;

    /// @brief Determines how often the road tile has to rotated by 90 degrees to fit the connected roads.
    /// @return A value beween 0 and 4 that specifies how often the road tile has to be rotated.
    int getRotation() const;

    /// @brief Determines the road type based on the connections
    /// @return The determined road type
    RoadType getType() const;

    /// @brief Returns the destination of the connection in the specified direction.
    /// @param dir The direction.
    /// @return The destination of the connection
    const glm::ivec2& getDestination(Direction dir) const;
};

struct RoadGraph {  
    std::unordered_map<glm::ivec2, RoadGraphNode> nodes;

    void insertNode(const glm::ivec2& position);

    void updateNodeConnection(const glm::ivec2& position, Direction dir);  

    void clear();  

    std::vector<glm::ivec2> getRoute(const glm::ivec2& start, const glm::ivec2& end) const;
};
