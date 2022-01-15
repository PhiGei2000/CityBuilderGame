#include "misc/streetGraph.hpp"

int StreetGraph::getEdgeIndex(const glm::ivec2& position) {
    for (int i = 0; i < edges.size(); i++) {
        StreetGraphEdge& edge = edges[i];
        const glm::ivec2& start = edge.begin->position;
        const glm::ivec2& end = edge.end->position;

        glm::ivec2 edgeVector = end - start;
        glm::ivec2 diffVector = position - start;

        bool x = edgeVector.x != 0 ? (0 <= diffVector.x && diffVector.x < edgeVector.x) : start.x == position.x;
        bool y = edgeVector.y != 0 ? (0 <= diffVector.y && diffVector.y < edgeVector.y) : start.y == position.y;

        if (x && y) {
            return i;
        }
    }

    return -1;
}

StreetGraphNode* StreetGraph::splitEdge(StreetGraphEdge& edge, const glm::ivec2& position) {
    StreetGraphNode& splitNode = nodes.emplace_back(StreetGraphNodeType::EDGE_SEPARATOR, position);

    edges.emplace_back(&splitNode, edge.end);
    edge.end = &splitNode;

    return &splitNode;
}

StreetGraphNode* StreetGraph::getOrCreateNode(const glm::ivec2& position) {
    for (auto& node : nodes) {
        if (node.position == position) {
            return &node;
        }
    }

    int edgeIndex = getEdgeIndex(position);
    if (edgeIndex != -1) {
        return splitEdge(edges[edgeIndex], position);
    }

    nodes.emplace_back(StreetGraphNodeType::END, position);
    return &nodes.back();
}

StreetGraphNode* StreetGraph::getOrCreateNode(int x, int y) {
    return getOrCreateNode(glm::ivec2(x, y));
}

void StreetGraph::addEdge(const glm::ivec2& start, const glm::ivec2& end, bool xFirst) {
    StreetGraphNode* startNode = getOrCreateNode(start);
    StreetGraphNode* endNode = getOrCreateNode(end);

    if (start.x == end.x || start.y == end.y) {
        edges.emplace_back(startNode, endNode);
    }
    else {
        StreetGraphNode* curveNode;
        if (xFirst) {
            curveNode = getOrCreateNode(start.x, end.y);
        }
        else {
            curveNode = getOrCreateNode(end.x, start.y);
        }

        edges.emplace_back(startNode, curveNode);
        edges.emplace_back(curveNode, endNode);
    }
}
