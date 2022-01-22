#include "misc/streetGraph.hpp"

#include "misc/direction.hpp"

int StreetGraph::getEdgeIndex(const glm::ivec2& position) {
    for (int index = 0; index < edges.size(); index++) {
        const StreetGraphEdge& edge = edges[index];

        const glm::ivec2& start = edge.begin;
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

void StreetGraph::splitEdge(StreetGraphEdge& edge, const glm::ivec2& position) {
    nodes.try_emplace(position, StreetGraphNodeType::EDGE_SEPARATOR, position, 0);

    edges.emplace_back(position, edge.end);
    edge.end = position;
}

void StreetGraph::createNode(const glm::ivec2& position) {
    nodes.try_emplace(position, StreetGraphNodeType::END, position, 0);
}

void StreetGraph::createNode(int x, int y) {
    return createNode(glm::ivec2(x, y));
}

void StreetGraph::addEdge(const glm::ivec2& start, const glm::ivec2& end, bool xFirst) {
    createNode(start);
    createNode(end);

    if (start.x == end.x || start.y == end.y) {
        edges.emplace_back(start, end);
    }
    else {
        glm::ivec2 curvePos = xFirst ? glm::ivec2{start.x, end.y} : glm::ivec2{end.x, start.y};

        createNode(curvePos);
        edges.emplace_back(start, curvePos);
        edges.emplace_back(curvePos, end);
    }
}

void StreetGraph::updateNodes() {
    std::unordered_map<glm::ivec2, std::array<bool, 4>> nodeConnections;

    for (const auto& [edgeStart, edgeEnd] : edges) {
        Direction edgeDirection = misc::getDirection(edgeEnd - edgeStart);

        // add connections
        nodeConnections[edgeStart][static_cast<byte>(edgeDirection)] = true;
        nodeConnections[edgeEnd][static_cast<byte>(misc::getInverse(edgeDirection))] = true;
    }

    // update node type and rotation
    for (auto& [pos, node] : nodes) {
        const std::array<bool, 4>& connections = nodeConnections[pos];

        int connectionsCount =
            (connections[0] ? 1 : 0) +
            (connections[1] ? 1 : 0) +
            (connections[2] ? 1 : 0) +
            (connections[3] ? 1 : 0);

        switch (connectionsCount) {
        case 0:
            node.type = StreetGraphNodeType::END_NOT_CONNECTED;
            node.rotation = 0;
            break;
        case 1:
            node.type = StreetGraphNodeType::END;

            // default rotation connected bottom
            // north connected
            if (connections[static_cast<int>(Direction::NORTH)]) {
                node.rotation = 2;
            }
            // east connected
            else if (connections[static_cast<int>(Direction::EAST)]) {
                node.rotation = 3;
            }
            // south connected
            else if (connections[static_cast<int>(Direction::SOUTH)]) {
                node.rotation = 0;
            }
            // west connected
            else if (connections[static_cast<int>(Direction::WEST)]) {
                node.rotation = 1;
            }
            break;
        case 2:
            if (connections[static_cast<int>(Direction::NORTH)] && connections[static_cast<int>(Direction::SOUTH)]) {
                node.type = StreetGraphNodeType::EDGE_SEPARATOR;
                node.rotation = 0;
            }
            else if (connections[static_cast<int>(Direction::EAST)] && connections[static_cast<int>(Direction::WEST)]) {
                node.type = StreetGraphNodeType::EDGE_SEPARATOR;
                node.rotation = 1;
            }
            else {
                node.type = StreetGraphNodeType::CURVE;
                if (connections[static_cast<int>(Direction::NORTH)] && connections[static_cast<int>(Direction::EAST)]) {
                    node.rotation = 0;
                }
                else if (connections[static_cast<int>(Direction::EAST)] && connections[static_cast<int>(Direction::SOUTH)]) {
                    node.rotation = 1;
                }
                else if (connections[static_cast<int>(Direction::SOUTH)] && connections[static_cast<int>(Direction::WEST)]) {
                    node.rotation = 2;
                }
                else if (connections[static_cast<int>(Direction::WEST)] && connections[static_cast<int>(Direction::NORTH)]) {
                    node.rotation = 3;
                }
            }
            break;
        case 3:
                node.type = StreetGraphNodeType::T_CROSSING;
            if (connections[static_cast<int>(Direction::NORTH)] && connections[static_cast<int>(Direction::SOUTH)]) {
                node.rotation = 0;
            }
            else if (connections[static_cast<int>(Direction::EAST)] && connections[static_cast<int>(Direction::WEST)]) {
                node.type = StreetGraphNodeType::EDGE_SEPARATOR;
                node.rotation = 1;
            }
        }
    }
}
