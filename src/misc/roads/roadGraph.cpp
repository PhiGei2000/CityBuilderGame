#include "misc/roads/roadGraph.hpp"

#include "misc/utility.hpp"

RoadGraphEdge::RoadGraphEdge()
    : length(0) {
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

bool RoadGraphNode::connected(Direction dir) const {
    return edges[(int)dir].length > 0;
}

int RoadGraphNode::getRotation() const {
    RoadType type = getType();

    switch (type) {
        case RoadType::STRAIGHT:
            return connected(Direction::NORTH) ? 0 : 1;
        case RoadType::END: {
            int i = 0;
            while (!connected((Direction)i))
                i++;

            return i;
        }
        case RoadType::CURVE:
        case RoadType::CURVE_FULL: {
            int i = 0;
            while (!connected((Direction)i))
                i++;

            if (i != 0) {
                return i;
            }

            return connected(Direction::WEST) ? 3 : 0;
        }
        case RoadType::T_CROSSING: {
            int i = 0;
            while (connected((Direction)i))
                i++;

            return i - 3;
        }
        default:
            return 0;
    }
}

RoadType RoadGraphNode::getType() const {
    int connectionsCount = (connected(Direction::NORTH) ? 1 : 0) + (connected(Direction::EAST) ? 1 : 0) + (connected(Direction::SOUTH) ? 1 : 0) + (connected(Direction::WEST) ? 1 : 0);

    switch (connectionsCount) {
        case 0:
            return RoadType::NOT_CONNECTED;
        case 1:
            return RoadType::END;
        case 2:
            if (connected(Direction::NORTH) == connected(Direction::SOUTH)) {
                return RoadType::STRAIGHT;
            }
            else {
                return RoadType::CURVE;
            }
        case 3:
            return RoadType::T_CROSSING;
        case 4:
            return RoadType::CROSSING;
        default:
            return RoadType::UNDEFINED;
    }
}

void RoadGraph::insertNode(const glm::ivec2& position) {
    if (!nodes.contains(position)) {
        nodes[position] = RoadGraphNode(position);
    }
}

void RoadGraph::updateNodeConnection(const glm::ivec2& start, Direction dir) {
    if (nodes.size() < 2) {
        return;
    }

    const glm::ivec2& direction = DirectionVectors[dir];
    glm::ivec2 pos = start + direction;

    while (!nodes.contains(pos)) {
        if (!(utility::inRange(pos.x, 0, Configuration::worldSize) && utility::inRange(pos.y, 0, Configuration::worldSize))) {
            return;
        }

        pos += direction;
    }

    nodes[start].edges[(int)dir] = RoadGraphEdge(start, pos);
    nodes[pos].edges[((int)dir + 2) % 4] = RoadGraphEdge(start, pos);
}

void RoadGraph::clear() {
    nodes.clear();
}
