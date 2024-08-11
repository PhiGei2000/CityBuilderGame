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
#include "misc/roads/roadGraph.hpp"

#include "misc/utility.hpp"

#include <iostream>
#include <stdexcept>

using Node = RoadGraph::RoadGraphNode;
using Edge = RoadGraph::RoadGraphEdge;

bool RoadGraph::adjacent(const Node& x, const Node& y) const {
    for (const auto& [edge, _] : edges) {
        const auto& [start, end] = edge;
        if (start == x && end == y) {
            return true;
        }
    }

    return false;
}

std::unordered_set<Node> RoadGraph::neighbours(const Node& x) const {
    std::unordered_set<Node> neighbours;

    for (const auto& [edge, _] : edges) {
        if (edge.first == x) {
            neighbours.insert(edge.second);
        }
    }

    return neighbours;
}

bool RoadGraph::addNode(const Node& x, const NodeData& data) {
    bool exists = nodes.contains(x);
    nodes[x] = data;

    return !exists;
}

bool RoadGraph::removeNode(const Node& x) {
    const std::unordered_set<Node>& neighbours = this->neighbours(x);
    for (const Node& neighbour : neighbours) {
        // remove edges if the node is connected
        removeEdge(Edge(x, neighbour));
        removeEdge(Edge(neighbour, x));
    }

    return (bool)nodes.erase(x);
}

void RoadGraph::updateNodeData(const Node& x, const NodeData& data) {
    nodes.at(x) = data;
}

bool RoadGraph::addEdge(const Node& x, const Node& y, const EdgeData& data) {
    if (!(nodes.contains(x) && nodes.contains(y))) {
        throw std::runtime_error("At least one node is not in the node list");
    }

    Edge e{x, y};
    bool exists = edges.contains(e);
    edges[e] = data;

    return !exists;
}

bool RoadGraph::removeEdge(const Edge& e) {
    return (bool)edges.erase(e);
}

void RoadGraph::updateEdgeData(const RoadGraphEdge& e, const EdgeData& data) {
    edges.at(e) = data;
}

void RoadGraph::clear() {
    nodes.clear();
    edges.clear();
}

const std::unordered_map<Node, RoadGraph::NodeData>& RoadGraph::getNodes() const {
    return nodes;
}

const std::unordered_map<Edge, RoadGraph::EdgeData>& RoadGraph::getEdges() const {
    return edges;
}