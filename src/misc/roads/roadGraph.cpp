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

#include <stdexcept>
#include <iostream>

using Node = RoadGraph::RoadGraphNode;
using Edge = RoadGraph::RoadGraphEdge;


RoadGraph::RoadGraph(const std::unordered_set<Node>& nodes, const std::unordered_set<Edge>& edges)
    : nodes(nodes) {

    for (const auto& [x, y] : edges) {
        addNode(x);
        addNode(y);

        addEdge(x, y);
    }
}

bool RoadGraph::adjacent(const Node& x, const Node& y) const {
    for (const Edge& edge : edges) {
        if (edge.first == x && edge.second == y) {
            return true;
        }
    }

    return false;
}

std::unordered_set<Node> RoadGraph::neighbours(const Node& x) const {
    std::unordered_set<Node> neighbours;

    for (const Edge& edge : edges) {
        if (edge.first == x) {
            neighbours.insert(edge.second);
        }
    }

    return neighbours;
}

bool RoadGraph::addNode(const Node& x) {
    return nodes.insert(x).second;
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

bool RoadGraph::addEdge(const Node& x, const Node& y) {
    if (!(nodes.contains(x) && nodes.contains(y))) {
        throw std::runtime_error("At least one node is not in the node list");
    }

    return edges.emplace(x, y).second;
}

bool RoadGraph::removeEdge(const Edge& e) {
    return (bool)edges.erase(e);
}

void RoadGraph::clear() {
    nodes.clear();
    edges.clear();
}

const std::unordered_set<Node>& RoadGraph::getNodes() const {
    return nodes;
}

const std::unordered_set<Edge>& RoadGraph::getEdges() const {
    return edges;
}