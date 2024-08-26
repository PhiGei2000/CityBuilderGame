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
#include "misc/roads/path.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>

using RoadPath = Path;

template<>
struct std::hash<std::pair<glm::ivec2, glm::ivec2>> {
    inline size_t operator()(const std::pair<glm::ivec2, glm::ivec2>& p) const noexcept {
        return std::hash<glm::ivec4>().operator()(glm::ivec4(p.first, p.second));
    }
};

struct RoadGraph {
  public:
    using RoadGraphNode = glm::ivec2;
    struct NodeData {
        std::array<std::array<RoadPath, 4>, 4> paths;
    };

    using RoadGraphEdge = std::pair<RoadGraphNode, RoadGraphNode>;
    using EdgeData = RoadPath;

  private:
    std::unordered_map<glm::ivec2, NodeData> nodes;
    std::unordered_map<RoadGraphEdge, EdgeData> edges;

  public:
    bool adjacent(const RoadGraphNode& x, const RoadGraphNode& y) const;

    std::unordered_set<RoadGraphNode> neighbours(const RoadGraphNode& x) const;

    bool addNode(const RoadGraphNode& x, const NodeData& data = NodeData());

    bool removeNode(const RoadGraphNode& x);

    void updateNodeData(const RoadGraphNode& x, const NodeData& data);

    bool addEdge(const RoadGraphNode& x, const RoadGraphNode& y, const EdgeData& data = {});

    bool removeEdge(const RoadGraphEdge& e);

    void updateEdgeData(const RoadGraphEdge& e, const EdgeData& data);

    void clear();

    const std::unordered_map<RoadGraphNode, NodeData>& getNodes() const;

    const std::unordered_map<RoadGraphEdge, EdgeData>& getEdges() const;
};
