#include "misc/streetGraph.hpp"

constexpr bool StreetGraphNode::connected(Direction direction) const {
    return connections[static_cast<int>(direction)];
}

int StreetGraphEdge::length() const {
    glm::ivec2 dr = end - start;
    return dr.x + dr.y;
}

int StreetGraph::getEdge(const glm::ivec2& position) {
    for (int index = 0; index < edges.size(); index++) {
        const StreetGraphEdge& edge = edges[index];

        const glm::ivec2& start = edge.start;
        const glm::ivec2& end = edge.end;

        glm::ivec2 edgeVector = end - start;
        glm::ivec2 diffVector = position - start;

        bool x = edgeVector.x != 0 ? (0 <= diffVector.x && diffVector.x < edgeVector.x) : start.x == position.x;
        bool y = edgeVector.y != 0 ? (0 <= diffVector.y && diffVector.y < edgeVector.y) : start.y == position.y;

        if (x && y) {
            return index;
        }
    }

    return -1;
}

void StreetGraph::splitEdge(int edgeIndex, const glm::ivec2& position) {
    auto [it, success] = nodes.try_emplace(position, position);

    if (success) {
        // get edge
        const StreetGraphEdge& edge = edges[edgeIndex];
        Direction edgeDirection = misc::getDirection(edge.end - edge.start);
        StreetGraphNode& node = (*it).second;

        // update node connections
        if (edgeDirection == Direction::NORTH || edgeDirection == Direction::SOUTH) {
            node.connections[0] = node.connections[2] = true;
        }
        else {
            node.connections[1] = node.connections[3] = true;
        }

        glm::ivec2 end = edge.end;
        edges[edgeIndex].end = position;
        addEdge(position, end);
    }
}

void StreetGraph::createNode(const glm::ivec2& position) {
    nodes.try_emplace(position, position);
}

void StreetGraph::createNode(int x, int y) {
    return createNode(glm::ivec2(x, y));
}

constexpr std::array<glm::ivec2, 4> StreetGraph::getNeighborPositions(const glm::ivec2& position) {
    return std::array<glm::ivec2, 4>{
        position + DirectionVectors[0],
        position + DirectionVectors[1],
        position + DirectionVectors[2],
        position + DirectionVectors[3],
    };
}

void StreetGraph::addEdge(const glm::ivec2& start, const glm::ivec2& end, bool xFirst) {
    if (start.x == end.x || start.y == end.y) {
        int edgeIndex;

        // if start node on another edge split edge
        if ((edgeIndex = getEdge(start)) != -1) {
            splitEdge(edgeIndex, start);
        }
        // if not create new simple node
        else {
            createNode(start);
        }

        // if end node on another edge split edge
        if ((edgeIndex = getEdge(end)) != -1) {
            splitEdge(edgeIndex, end);
        }
        else {
            createNode(end);
        }

        // create edge
        const glm::ivec2 edgeVector = end - start;
        Direction edgeDir = misc::getDirection(end - start);
        const glm::ivec2 edgeDirVector = DirectionVectors[static_cast<int>(edgeDir)];

        // search for nodes on the new edge or edge intersections
        std::vector<glm::ivec2> nodesOnEdge;
        glm::ivec2 pos = start;
        do {
            auto it = nodes.find(pos);
            if (it != nodes.end()) {
                nodesOnEdge.push_back(pos);
            }

            int edgeIndex = getEdge(pos);
            if (edgeIndex != -1) {
                splitEdge(edgeIndex, pos);
                nodesOnEdge.push_back(pos);
            }

            pos += edgeDirVector;
        } while (pos != end + edgeDirVector);

        // connect nodes
        for (int i = 1; i < nodesOnEdge.size(); i++) {
            edges.emplace_back(nodesOnEdge[i - 1], nodesOnEdge[i]);

            // update node connections
            if (nodes[nodesOnEdge[i - 1]].position != end) {
                nodes[nodesOnEdge[i - 1]].connections[static_cast<int>(edgeDir)] = true;
            }

            if (nodes[nodesOnEdge[i]].position != start) {
                nodes[nodesOnEdge[i]].connections[static_cast<int>(misc::getInverse(edgeDir))] = true;
            }
        }
    }
    else {
        glm::ivec2 curvePos = xFirst ? glm::ivec2{start.x, end.y} : glm::ivec2{end.x, start.y};

        createNode(curvePos);
        addEdge(start, curvePos);
        addEdge(curvePos, end);
    }
}

void StreetGraph::updateNodes() {
    // merge edges
    for (auto it = nodes.begin(); it != nodes.end();) {
        const auto& [pos, node] = *it;
        // if node is edge
        if (node.connections[0] && !node.connections[1] && node.connections[2] && !node.connections[3] ||
            !node.connections[0] && node.connections[1] && !node.connections[2] && node.connections[3]) {
            // find the edges connected to this node
            int edge1 = -1, edge2 = -1;

            StreetGraphEdge edge;
            int i = 0;
            while ((edge1 == -1 || edge2 == -1) && i < edges.size()) {
                if (edge1 == -1) {
                    if (edges[i].start == pos) {
                        edge.start = edges[i].end;
                        edge1 = i;
                    }
                    else if (edges[i].end == pos) {
                        edge.start = edges[i].start;
                        edge1 = i;
                    }
                }
                else if (edge2 == -1) {
                    if (edges[i].start == pos) {
                        edge.end = edges[i].end;
                        edge2 = i;
                    }
                    else if (edges[i].end == pos) {
                        edge.end = edges[i].start;
                        edge2 = i;
                    }
                }

                i++;
            }

            // delete edges
            edges.erase(edges.begin() + glm::max(edge1, edge2));
            edges.erase(edges.begin() + glm::min(edge1, edge2));

            // create new edge and delete node
            edges.push_back(edge);
            it = nodes.erase(it);
        }
        else {
            it++;
        }
    }
}
