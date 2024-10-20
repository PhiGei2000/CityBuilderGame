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

#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>

using RoadPath = Path;

template<>
struct std::hash<std::pair<glm::ivec2, glm::ivec2>> {
    inline size_t operator()(const std::pair<glm::ivec2, glm::ivec2>& p) const noexcept {
        return std::hash<glm::ivec4>().operator()(glm::ivec4(p.first, p.second));
    }
};

template<typename T>
concept GraphDataTypes = requires {
    typename T::NodeType;
    typename T::EdgeType;

    typename T::NodeDataType;
    typename T::EdgeDataType;
};

template<GraphDataTypes dataTypes>
struct Graph {
    using NodeType = dataTypes::NodeType;
    using EdgeType = dataTypes::EdgeType;
    using NodeDataType = dataTypes::NodeDataType;
    using EdgeDataType = dataTypes::EdgeDataType;

  protected:
    std::unordered_map<NodeType, NodeDataType> nodes;
    std::unordered_map<EdgeType, EdgeDataType> edges;

  public:
    inline virtual bool adjacent(const NodeType& x, const NodeType& y) const {
        for (const auto& [edge, _] : edges) {
            const auto& [start, end] = edge;
            if (start == x && end == y) {
                return true;
            }
        }

        return false;
    }

    virtual std::unordered_set<NodeType> neighbours(const NodeType& x) const {
        std::unordered_set<NodeType> neighbours;

        for (const auto& [edge, _] : edges) {
            if (edge.first == x) {
                neighbours.insert(edge.second);
            }
        }

        return neighbours;
    }

    virtual bool addNode(const NodeType& x, const NodeDataType& data = NodeDataType()) {
        bool exists = nodes.contains(x);
        nodes[x] = data;

        return !exists;
    }

    virtual bool removeNode(const NodeType& x) {
        const std::unordered_set<NodeType>& neighbours = this->neighbours(x);
        for (const NodeType& neighbour : neighbours) {
            // remove edges if the node is connected
            removeEdge(EdgeType(x, neighbour));
            removeEdge(EdgeType(neighbour, x));
        }

        return (bool)nodes.erase(x);
    }

    virtual void updateNodeData(const NodeType& x, const NodeDataType& data) {
        nodes.at(x) = data;
    }

    virtual bool addEdge(const NodeType& x, const NodeType& y, const EdgeDataType& data = {}) {
        if (!(nodes.contains(x) && nodes.contains(y))) {
            throw std::runtime_error("At least one node is not in the node list");
        }

        EdgeType e{x, y};
        bool exists = edges.contains(e);
        edges[e] = data;

        return !exists;
    }

    virtual bool removeEdge(const EdgeType& e) {
        return (bool)edges.erase(e);
    }

    virtual void updateEdgeData(const EdgeType& e, const EdgeDataType& data) {
        edges.at(e) = data;
    }

    virtual void clear() {
        nodes.clear();
        edges.clear();
    }

    virtual const std::unordered_map<NodeType, NodeDataType>& getNodes() const {
        return nodes;
    }

    virtual const std::unordered_map<EdgeType, EdgeDataType>& getEdges() const {
        return edges;
    }

    virtual const NodeDataType& getNodeData(const NodeType& node) const {
        return nodes.at(node);
    }

    virtual const EdgeDataType& getEdgeData(const NodeType& x, const NodeType& y) const {
        return edges.at(std::make_pair(x, y));
    }
};

struct RoadGraphDataTypes {
  public:
    using NodeType = glm::ivec2;
    struct NodeDataType {
        std::array<std::array<RoadPath, 4>, 4> paths;
    };

    using EdgeType = std::pair<NodeType, NodeType>;
    using EdgeDataType = RoadPath;
};

using RoadGraph = Graph<RoadGraphDataTypes>;
