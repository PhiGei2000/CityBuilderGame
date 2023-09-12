#include "misc/roads/roadGraph.hpp"

#include "misc/utility.hpp"

#include <set>
#include <unordered_set>

using IntersectionInfo = RoadGraphEdge::IntersectionInfo;

constexpr bool std::less<glm::ivec2>::operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) const {
    return lhs.x < rhs.x || lhs.y < rhs.y;
}

IntersectionInfo::IntersectionInfo()
    : intersects(false), parallel(false), position(-1) {
}

IntersectionInfo::IntersectionInfo(bool intersects, bool parallel, const glm::ivec2& position)
    : intersects(intersects), parallel(parallel), position(position) {
}

RoadGraphEdge::RoadGraphEdge()
    : length(0), start(-1), end(-1) {
}

RoadGraphEdge::RoadGraphEdge(const glm::ivec2& start, const glm::ivec2& end) {
    length = glm::length(end - start);

    if (start.y == end.y) {
        if (start.x < end.x) {
            this->start = start;
            this->end = end;
        }
        else {
            this->start = end;
            this->end = start;
        }
    }
    else if (start.x == end.x) {
        if (start.y < end.y) {
            this->start = start;
            this->end = end;
        }
        else {
            this->start = end;
            this->end = start;
        }
    }
    else {
        throw std::invalid_argument("No diagonal edges allowed");
    }
}

template<>
IntersectionInfo RoadGraphEdge::getParallelEdgeIntersectionPosition<1>(const RoadGraphEdge& other) const {
    IntersectionInfo info(false, true, glm::ivec2(-1));

    if (start.y == other.start.y) {
        if (utility::inRange(other.start.x, start.x, end.x)) {
            info.intersects = true;
            info.position = other.start;
        }
        else if (utility::inRange(other.end.x, start.x, end.x)) {
            info.intersects = true;
            info.position = start;
        }
    }

    return info;
}

template<>
IntersectionInfo RoadGraphEdge::getParallelEdgeIntersectionPosition<2>(const RoadGraphEdge& other) const {
    IntersectionInfo info = {false, true, glm::ivec2(-1)};

    if (start.x == other.start.x) {
        if (utility::inRange(other.start.y, start.y, end.y)) {
            info.intersects = true;
            info.position = other.start;
        }
        else if (utility::inRange(other.end.y, start.y, end.y)) {
            info.intersects = true;
            info.position = start;
        }
    }

    return info;
}

bool RoadGraphEdge::contains(const glm::ivec2& position) const {
    if (start.x == end.x) {
        // east <-> west
        return start.x == position.x && utility::inRange(position.y, start.y, end.y);
    }
    else if (start.y == end.y) {
        // north <-> south
        return start.y == position.y && utility::inRange(position.x, start.x, end.x);
    }

    return false;
}

IntersectionInfo RoadGraphEdge::intersects(const RoadGraphEdge& other) const {
    bool horizontal = isHorzontal();
    bool otherHorizontal = other.isHorzontal();

    IntersectionInfo info;
    info.parallel = horizontal == otherHorizontal;

    if (horizontal) {
        if (!otherHorizontal) {
            info.intersects = utility::inRange(other.start.x, start.x, end.x);

            if (info.intersects) {
                info.position = glm::ivec2(other.start.x, start.y);
            }
        }
        else {
            if (start.y == other.start.y) {
                info = getParallelEdgeIntersectionPosition<1>(other);
            }
        }
    }
    else {
        if (otherHorizontal) {
            info.intersects = utility::inRange(other.start.y, start.y, end.y);

            if (info.intersects) {
                info.position = glm::ivec2(start.x, other.start.y);
            }
        }
        else {
            if (start.x == other.start.x) {
                info = getParallelEdgeIntersectionPosition<2>(other);
            }
        }
    }

    return info;
}

bool RoadGraphEdge::isHorzontal() const {
    return start.y == end.y;
}

bool RoadGraphEdge::isVertical() const {
    return start.x == end.x;
}

Direction RoadGraphEdge::getDirection() const {
    return utility::getDirection(end - start);
}

bool RoadGraphEdge::operator==(const RoadGraphEdge& other) const {
    return start == other.start && end == other.end;
}

bool RoadGraphEdge::operator!=(const RoadGraphEdge& other) const {
    return !(this->operator==(other));
}

RoadGraphNode::RoadGraphNode()
    : position(-1) {
}

RoadGraphNode::RoadGraphNode(const glm::ivec2& position)
    : position(position) {
}

RoadGraphEdge& RoadGraphNode::operator[](const Direction& dir) {
    return edges[(int)dir];
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

const glm::ivec2& RoadGraphNode::getDestination(Direction dir) const {
    if (!connected(dir))
        return position;

    return edges[(int)dir].start == position ? edges[(int)dir].end : edges[(int)dir].start;
}

void RoadGraph::insertNode(const glm::ivec2& position) {
    if (!nodes.contains(position)) {
        nodes[position] = RoadGraphNode(position);

#if DEBUG
        std::cout << "Node at: (" << position << ") inserted" << std::endl;
#endif
    }
}

void RoadGraph::updateNodeConnection(const glm::ivec2& start, Direction dir, bool createNew) {
    if (nodes.size() < 2) {
        return;
    }

    const glm::ivec2& direction = DirectionVectors[dir];
    glm::vec2 pos = start + direction;

    while (!nodes.contains(pos)) {
        if (!(utility::inRange<float>(pos.x, 0, Configuration::chunkSize) && utility::inRange<float>(pos.y, 0, Configuration::chunkSize))) {
            return;
        }

        pos += direction;
    }

    const RoadGraphEdge& edge = RoadGraphEdge(start, pos);
    Direction edgeDir = edge.getDirection();

    if (createNew) {
        nodes[edge.start][edgeDir] = edge;
        nodes[edge.end][-edgeDir] = edge;
    }
    else {
        // update the connection only if the connection already exisits
        if (nodes[edge.end][-edgeDir].length > 0 || nodes[edge.start][edgeDir].length > 0) {
            nodes[edge.start][edgeDir] = edge;
            nodes[edge.end][-edgeDir] = edge;
        }
    }
}

void RoadGraph::updateNodeConnections(const glm::ivec2& position, bool createNew) {
    for (Direction dir = Direction::NORTH; dir != Direction::UNDEFINED; dir++) {
        updateNodeConnection(position, dir, createNew);
    }
}

void RoadGraph::connectNodes(const glm::ivec2& start, const glm::ivec2& end) {
    const RoadGraphEdge& newEdge = RoadGraphEdge(start, end);
    Direction dir = utility::getDirection(newEdge.end - newEdge.start);

    std::set<glm::ivec2> nodesOnNewEdge;

    // determine existing nodes and edge intersections on the new edge
    for (auto& [pos, node] : nodes) {
        if (newEdge.contains(pos)) {
            nodesOnNewEdge.insert(pos);
        }

        for (auto& edge : node.edges) {
            IntersectionInfo info = newEdge.intersects(edge);
            if (info.intersects) {
                insertNode(info.position);
                updateNodeConnections(info.position, false);

                nodesOnNewEdge.insert(info.position);
            }
        }
    }

    // connect nodes
    for (auto it = nodesOnNewEdge.begin(); std::next(it) != nodesOnNewEdge.end(); it++) {
        const glm::ivec2& start = *it;
        const glm::ivec2& end = *std::next(it);
        const RoadGraphEdge& edge = RoadGraphEdge(start, end);

        #if DEBUG
        std::cout << "Create new edge from: " << edge.start << " to:" << edge.end << std::endl;
        #endif

        nodes[start][dir] = edge;
        nodes[end][-dir] = edge;
    }
}

void RoadGraph::clear() {
    nodes.clear();
}

std::vector<glm::ivec2> RoadGraph::getRoute(const glm::ivec2& start, const glm::ivec2& end) const {
    struct Node {
        int distance = INT_MAX;
        Node* last = nullptr;

        glm::ivec2 pos;
    };

    std::unordered_set<glm::ivec2> unvisitedNodes;
    for (const auto& [pos, _] : nodes) {
        unvisitedNodes.emplace(pos);
    }

    std::unordered_map<glm::ivec2, Node> nodeDistances;
    nodeDistances[end] = {0, nullptr, end};

    Node* currentNode = &nodeDistances[end];
    Node* lastNode = &nodeDistances[end];

    // while any nodes are unvisited
    while (unvisitedNodes.size() > 0) {
        const glm::ivec2 currentPos = currentNode->pos;
        if (currentPos == start)
            break;

        // node was visited
        unvisitedNodes.erase(currentPos);

        const RoadGraphNode& graphNode = nodes.at(currentPos);

        // calculate distance for unvisited and select node with min distance
        int minDistance = INT_MAX;
        Node* nextNode;

        for (Direction dir = Direction::NORTH; dir != Direction::UNDEFINED; dir++) {
            if (currentNode != lastNode && graphNode.connected(dir)) {
                const glm::ivec2& destination = graphNode.getDestination(dir);

                int distanceSum = lastNode->distance + graphNode.edges[(int)dir].length;
                if (distanceSum < nodeDistances[destination].distance) {
                    nodeDistances[destination] = {distanceSum, &nodeDistances[currentPos], destination};

                    // select unvisited node with minimal distance for the next iteration
                    if (distanceSum < minDistance && unvisitedNodes.contains(destination)) {
                        nextNode = &nodeDistances[destination];
                        minDistance = distanceSum;
                    }
                }
            }
        }

        lastNode = currentNode;
        currentNode = nextNode;
    }

    // iterate through nodes and generate path
    std::vector<glm::ivec2> path;
    do {
        path.push_back(currentNode->pos);

        currentNode = currentNode->last;
    } while (currentNode->last != nullptr);

    return path;
}
