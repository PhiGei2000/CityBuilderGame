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

#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>

#include <unordered_set>

template<>
struct std::hash<std::pair<glm::ivec2, glm::ivec2>> {
  inline std::size_t operator()(const std::pair<glm::ivec2, glm::ivec2>& x) const noexcept {
    glm::ivec4 vec = glm::ivec4(x.first, x.second);

    return std::hash<glm::ivec4>{}(vec);
  }
};

struct RoadGraph {
  public:
    using RoadGraphNode = glm::ivec2;
    using RoadGraphEdge = std::pair<RoadGraphNode, RoadGraphNode>;

  private:
    std::unordered_set<RoadGraphNode> nodes;
    std::unordered_set<RoadGraphEdge> edges;

  public:
    RoadGraph(const std::unordered_set<RoadGraphNode>& nodes = {}, const std::unordered_set<RoadGraphEdge>& edges = {});

    bool adjacent(const RoadGraphNode& x, const RoadGraphNode& y) const;

    std::unordered_set<RoadGraphNode> neighbours(const RoadGraphNode& x) const;

    bool addNode(const RoadGraphNode& x);

    bool removeNode(const RoadGraphNode& x);

    bool addEdge(const RoadGraphNode& x, const RoadGraphNode& y);

    bool removeEdge(const RoadGraphEdge& e);

    void clear();

    const std::unordered_set<RoadGraphNode>& getNodes() const;

    const std::unordered_set<RoadGraphEdge>& getEdges() const;
};

