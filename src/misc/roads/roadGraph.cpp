#include "misc/roads/roadGraph.hpp"

#include "misc/utility.hpp"

RoadGraphEdge::RoadGraphEdge()
    : length(INT_MAX) {
}

RoadGraphEdge::RoadGraphEdge(const glm::ivec2& start, const glm::ivec2& end)
    : start(start), end(end) {
    length = glm::length(end - start);
}

bool RoadGraphEdge::contains(const glm::ivec2& position) const {
    if (start.x == end.x) {
        // east <-> west
        return utility::inRange(position.y, glm::min(start.y, end.y), glm::max(start.y, end.y));
    }
    else if (start.y == end.y) {
        // north <-> south
        return utility::inRange(position.x, glm::min(start.x, end.x), glm::max(start.x, end.x));
    }

    return false;
}

RoadGraphNode::RoadGraphNode()
    : position(-1) {
}

RoadGraphNode::RoadGraphNode(const glm::ivec2& position)
    : position(position) {
}

void RoadGraph::insertNode(const glm::ivec2& position) {
    if (!nodes.contains(position)) {
        nodes[position] = RoadGraphNode(position);
    }
}

void RoadGraph::updateNodeConnection(const glm::ivec2& start, Direction dir) {
    const glm::ivec2& direction = DirectionVectors[dir];
    glm::ivec2 pos = start + direction;

    while (!nodes.contains(pos)) {
        assert((0 <= pos.x && pos.x <= Configuration::worldSize) && (0 <= pos.y && pos.y < Configuration::worldSize));

        pos += direction;
    }

    nodes[start].edges[(int)dir] = RoadGraphEdge(start, pos);
    nodes[pos].edges[((int)dir + 2) % 4] = RoadGraphEdge(pos, start);
}
