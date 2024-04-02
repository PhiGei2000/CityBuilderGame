/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "misc/direction.hpp"
#include "misc/roads/roadTypes.hpp"

#include <array>
#include <map>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace std {
    template<>
    struct less<glm::ivec2> {
        constexpr bool operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) const;
    };
} // namespace std

struct RoadGraphEdge {
  private:
    glm::ivec2 position1;
    glm::ivec2 position2;
    bool bidirectional;

  public:
    RoadGraphEdge(const glm::ivec2& pos1, const glm::ivec2& pos2);

    std::pair<glm::ivec2, glm::uvec2> getPosition_Direction() const;
    unsigned int getLength() const;

    static bool intersect(const RoadGraphEdge& e1, const RoadGraphEdge& e2, glm::ivec2& intersectionPoint);
};

// struct RoadGraphNode {
//     glm::ivec2 position;
//     std::array<RoadGraphEdge, 4> edges;
//
//     RoadGraphNode();
//     RoadGraphNode(const glm::ivec2& position);
//
//     RoadGraphEdge& operator[](const Direction& dir);
//
//     /// @brief Checks if the node has a connection in the specified direction.
//     /// @param dir The direction to check
//     /// @return `true` if the node is connected, `false` otherwise
//     bool connected(Direction dir) const;
//
//     /// @brief Determines how often the road tile has to rotated by 90 degrees to fit the connected roads.
//     /// @return A value beween 0 and 4 that specifies how often the road tile has to be rotated.
//     int getRotation() const;
//
//     /// @brief Determines the road type based on the connections
//     /// @return The determined road type
//     RoadTileType getType() const;
//
//     /// @brief Returns the destination of the connection in the specified direction.
//     /// @param dir The direction.
//     /// @return The destination of the connection
//     const glm::ivec2& getDestination(Direction dir) const;
// };

// struct RoadGraph {
//   public:
//     // TODO: Rework this: remove the RoadGraphNode and RoadGraphEdge struct and replace it with an vector/array which contains the positions of the connected nodes
//     std::unordered_map<glm::ivec2, RoadGraphNode> nodes;
//
//     /// @brief If no node exists at the specified position, a new node is created at the specified position.
//     /// @param position The position of the node
//     void insertNode(const glm::ivec2& position);
//
//     void updateNodeConnection(const glm::ivec2& position, Direction dir, bool createNew = true);
//     void updateNodeConnections(const glm::ivec2& position, bool createNew = true);
//
//     void connectNodes(const glm::ivec2& start, const glm::ivec2& end);
//
//     void clear();
//
//     /// @brief Adds the nodes and edges of the given `RoadGraph` into the graph
//     /// @param other The `RoadGraph` that should be added
//     void merge(const RoadGraph& other);
//
//     std::vector<glm::ivec2> getRoute(const glm::ivec2& start, const glm::ivec2& end) const;
// };

struct RoadGraph {
  private:
    using edgeCollection = std::multimap<glm::ivec2, glm::ivec2>;

    /// @brief A map which represents the nodes of the road graph. The keys are the positions of the nodes and the array holds the position of the connected node for each direction. If no node is connected in a direction, the stored position is the position of the node itself.
    edgeCollection connections;

  public:
    /// @brief Checks if there is a node at the specified position
    /// @param pos The position to check
    /// @return `true` if a node was found
    bool exists(const glm::ivec2& pos) const;

    /// @brief Checks if two nodes are connected
    /// @param pos1 Position of the first node
    /// @param pos2 Position of the second node
    /// @return `true` if the nodes are connected
    bool connected(const glm::ivec2& pos1, const glm::ivec2& pos2) const;

    /// @brief Returns the edge which connects the two positions. If the nodes are not connected
    /// @param pos1
    /// @param pos2
    /// @return
    RoadGraphEdge getEdge(const glm::ivec2& pos1, const glm::ivec2& pos2) const;

    /// @brief Inserts a node and connects it to itself, if it does not exitst
    /// @param position The position of the node
    void addNode(const glm::ivec2& position);

    /// @brief Creates a connection between two nodes
    /// @param position1 The position of the first node
    /// @param position2 The position of the second node
    /// @return `true` if the graph was modified
    bool addEdge(const glm::ivec2& position1, const glm::ivec2& position2, bool bidirectonal = true);
};
