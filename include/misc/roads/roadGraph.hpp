#pragma once
#include "misc/direction.hpp"
#include "misc/roads/roadTypes.hpp"

#include <array>
#include <queue>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace std {
    template<>
    struct less<glm::ivec2> {
        constexpr bool operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) const;
    };
}

struct RoadGraphEdge {
  public:
    struct IntersectionInfo {
        bool intersects;
        bool parallel;

        glm::ivec2 position;

        IntersectionInfo();
        IntersectionInfo(bool intersects, bool parallel, const glm::ivec2& position);
    };

  private:
      template<int coordinate>
    IntersectionInfo getParallelEdgeIntersectionPosition(const RoadGraphEdge& other) const;

  public:
    glm::ivec2 start, end;
    int length;

    RoadGraphEdge();
    RoadGraphEdge(const glm::ivec2& start, const glm::ivec2& end);

    bool contains(const glm::ivec2& position) const;

    IntersectionInfo intersects(const RoadGraphEdge& other) const;

    bool isHorzontal() const;
    bool isVertical() const;

    bool operator==(const RoadGraphEdge& other) const;
    bool operator!=(const RoadGraphEdge& other) const;
};

struct RoadGraphNode {
    glm::ivec2 position;
    std::array<RoadGraphEdge, 4> edges;

    RoadGraphNode();
    RoadGraphNode(const glm::ivec2& position);

    RoadGraphEdge& operator[](const Direction& dir);

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
  private:
  public:
    std::unordered_map<glm::ivec2, RoadGraphNode> nodes;

    /// @brief If no node exists at the specified position, a new node is created at the specified position.
    /// @param position The position of the node
    void insertNode(const glm::ivec2& position);

    void updateNodeConnection(const glm::ivec2& position, Direction dir, bool createNew = true);
    void updateNodeConnections(const glm::ivec2& position, bool createNew = true);

    void connectNodes(const glm::ivec2& start, const glm::ivec2& end);

    void clear();

    std::vector<glm::ivec2> getRoute(const glm::ivec2& start, const glm::ivec2& end) const;
};
