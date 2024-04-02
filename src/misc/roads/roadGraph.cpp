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

// #include <set>
// #include <unordered_set>

// using IntersectionInfo = RoadGraphEdge::IntersectionInfo;

constexpr bool std::less<glm::ivec2>::operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) const {
    long left = lhs.x + lhs.y << 8;
    long right = lhs.x + lhs.y << 8;

    return left < right;
}

// IntersectionInfo::IntersectionInfo()
//     : intersects(false), parallel(false), position(-1) {
// }

// IntersectionInfo::IntersectionInfo(bool intersects, bool parallel, const glm::ivec2& position)
//     : intersects(intersects), parallel(parallel), position(position) {
// }

// RoadGraphEdge::RoadGraphEdge()
//     : start(-INT_MAX), end(-INT_MAX) {
// }

// RoadGraphEdge::RoadGraphEdge(const glm::ivec2& start, const glm::ivec2& end) {
//     if (start.y == end.y) {
//         if (start.x < end.x) {
//             this->start = start;
//             this->end = end;
//         }
//         else {
//             this->start = end;
//             this->end = start;
//         }
//     }
//     else if (start.x == end.x) {
//         if (start.y < end.y) {
//             this->start = start;
//             this->end = end;
//         }
//         else {
//             this->start = end;
//             this->end = start;
//         }
//     }
//     else {
//         throw std::invalid_argument("No diagonal edges allowed");
//     }
// }

// template<>
// IntersectionInfo RoadGraphEdge::getParallelEdgeIntersectionPosition<1>(const RoadGraphEdge& other) const {
//     IntersectionInfo info(false, true, glm::ivec2(-1));

//     if (start.y == other.start.y) {
//         if (utility::inRange(other.start.x, start.x, end.x)) {
//             info.intersects = true;
//             info.position = other.start;
//         }
//         else if (utility::inRange(other.end.x, start.x, end.x)) {
//             info.intersects = true;
//             info.position = start;
//         }
//     }

//     return info;
// }

// template<>
// IntersectionInfo RoadGraphEdge::getParallelEdgeIntersectionPosition<2>(const RoadGraphEdge& other) const {
//     IntersectionInfo info = {false, true, glm::ivec2(-1)};

//     if (start.x == other.start.x) {
//         if (utility::inRange(other.start.y, start.y, end.y)) {
//             info.intersects = true;
//             info.position = other.start;
//         }
//         else if (utility::inRange(other.end.y, start.y, end.y)) {
//             info.intersects = true;
//             info.position = start;
//         }
//     }

//     return info;
// }

// bool RoadGraphEdge::contains(const glm::ivec2& position) const {
//     if (start.x == end.x) {
//         // east <-> west
//         return start.x == position.x && utility::inRange(position.y, start.y, end.y);
//     }
//     else if (start.y == end.y) {
//         // north <-> south
//         return start.y == position.y && utility::inRange(position.x, start.x, end.x);
//     }

//     return false;
// }

// IntersectionInfo RoadGraphEdge::intersects(const RoadGraphEdge& other) const {
//     bool horizontal = isHorzontal();
//     bool otherHorizontal = other.isHorzontal();

//     IntersectionInfo info;
//     info.parallel = horizontal == otherHorizontal;

//     if (horizontal) {
//         if (!otherHorizontal) {
//             info.intersects = utility::inRange(other.start.x, start.x, end.x);

//             if (info.intersects) {
//                 info.position = glm::ivec2(other.start.x, start.y);
//             }
//         }
//         else {
//             if (start.y == other.start.y) {
//                 info = getParallelEdgeIntersectionPosition<1>(other);
//             }
//         }
//     }
//     else {
//         if (otherHorizontal) {
//             info.intersects = utility::inRange(other.start.y, start.y, end.y);

//             if (info.intersects) {
//                 info.position = glm::ivec2(start.x, other.start.y);
//             }
//         }
//         else {
//             if (start.x == other.start.x) {
//                 info = getParallelEdgeIntersectionPosition<2>(other);
//             }
//         }
//     }

//     return info;
// }

// bool RoadGraphEdge::isHorzontal() const {
//     return start.y == end.y;
// }

// bool RoadGraphEdge::isVertical() const {
//     return start.x == end.x;
// }

// Direction RoadGraphEdge::getDirection() const {
//     return utility::getDirection(end - start);
// }

// int RoadGraphEdge::getLength() const {
//     return glm::length(end - start);
// }

// bool RoadGraphEdge::operator==(const RoadGraphEdge& other) const {
//     return start == other.start && end == other.end;
// }

// bool RoadGraphEdge::operator!=(const RoadGraphEdge& other) const {
//     return !(this->operator==(other));
// }

#pragma region RoadGraphNode
// RoadGraphNode::RoadGraphNode()
//     : position(-1) {
// }

// RoadGraphNode::RoadGraphNode(const glm::ivec2& position)
//     : position(position) {
// }

// RoadGraphEdge& RoadGraphNode::operator[](const Direction& dir) {
//     return edges[(int)dir];
// }

// bool RoadGraphNode::connected(Direction dir) const {
//     return edges[(int)dir].length > 0;
// }

// int RoadGraphNode::getRotation() const {
//     RoadTileType type = getType();

//     switch (type) {
//         case RoadTileType::STRAIGHT:
//             return connected(Direction::NORTH) ? 0 : 1;
//         case RoadTileType::END: {
//             int i = 0;
//             while (!connected((Direction)i))
//                 i++;

//             return i;
//         }
//         case RoadTileType::CURVE:
//         case RoadTileType::CURVE_FULL: {
//             int i = 0;
//             while (!connected((Direction)i))
//                 i++;

//             if (i != 0) {
//                 return i;
//             }

//             return connected(Direction::WEST) ? 3 : 0;
//         }
//         case RoadTileType::T_CROSSING: {
//             int i = 0;
//             while (connected((Direction)i))
//                 i++;

//             return i - 3;
//         }
//         default:
//             return 0;
//     }
// }

// RoadTileType RoadGraphNode::getType() const {
//     int connectionsCount = (connected(Direction::NORTH) ? 1 : 0) + (connected(Direction::EAST) ? 1 : 0) + (connected(Direction::SOUTH) ? 1 : 0) + (connected(Direction::WEST) ? 1 : 0);

//     switch (connectionsCount) {
//         case 0:
//             return RoadTileType::NOT_CONNECTED;
//         case 1:
//             return RoadTileType::END;
//         case 2:
//             if (connected(Direction::NORTH) == connected(Direction::SOUTH)) {
//                 return RoadTileType::STRAIGHT;
//             }
//             else {
//                 return RoadTileType::CURVE;
//             }
//         case 3:
//             return RoadTileType::T_CROSSING;
//         case 4:
//             return RoadTileType::CROSSING;
//         default:
//             return RoadTileType::UNDEFINED;
//     }
// }

// const glm::ivec2& RoadGraphNode::getDestination(Direction dir) const {
//     if (!connected(dir))
//         return position;

//     return edges[(int)dir].start == position ? edges[(int)dir].end : edges[(int)dir].start;
// }

// void RoadGraph::insertNode(const glm::ivec2& position) {
//     if (!nodes.contains(position)) {
//         nodes[position] = RoadGraphNode(position);

// #if DEBUG
//         std::cout << "Node at: (" << position << ") inserted" << std::endl;
// #endif
//     }
// }

// void RoadGraph::updateNodeConnection(const glm::ivec2& start, Direction dir, bool createNew) {
//     if (nodes.size() < 2) {
//         return;
//     }

//     const glm::ivec2& direction = DirectionVectors[dir];
//     glm::vec2 pos = start + direction;

//     while (!nodes.contains(pos)) {
//         // if (!(utility::inRange<float>(pos.x, 0, Configuration::chunkSize) && utility::inRange<float>(pos.y, 0, Configuration::chunkSize))) {
//         //     return;
//         // }

//         pos += direction;
//     }

//     const RoadGraphEdge& edge = RoadGraphEdge(start, pos);
//     Direction edgeDir = edge.getDirection();

//     if (createNew) {
//         nodes[edge.start][edgeDir] = edge;
//         nodes[edge.end][-edgeDir] = edge;
//     }
//     else {
//         // update the connection only if the connection already exisits
//         if (nodes[edge.end][-edgeDir].length > 0 || nodes[edge.start][edgeDir].length > 0) {
//             nodes[edge.start][edgeDir] = edge;
//             nodes[edge.end][-edgeDir] = edge;
//         }
//     }
// }

// void RoadGraph::updateNodeConnections(const glm::ivec2& position, bool createNew) {
//     for (Direction dir = Direction::NORTH; dir != Direction::UNDEFINED; dir++) {
//         updateNodeConnection(position, dir, createNew);
//     }
// }

// void RoadGraph::connectNodes(const glm::ivec2& start, const glm::ivec2& end) {
//     const RoadGraphEdge& newEdge = RoadGraphEdge(start, end);
//     Direction dir = utility::getDirection(newEdge.end - newEdge.start);

//     std::set<glm::ivec2> nodesOnNewEdge;

//     // determine existing nodes and edge intersections on the new edge
//     for (auto& [pos, node] : nodes) {
//         if (newEdge.contains(pos)) {
//             nodesOnNewEdge.insert(pos);
//         }

//         for (auto& edge : node.edges) {
//             IntersectionInfo info = newEdge.intersects(edge);
//             if (info.intersects) {
//                 insertNode(info.position);
//                 updateNodeConnections(info.position, false);

//                 nodesOnNewEdge.insert(info.position);
//             }
//         }
//     }

//     // connect nodes
//     for (auto it = nodesOnNewEdge.begin(); std::next(it) != nodesOnNewEdge.end(); it++) {
//         const glm::ivec2& start = *it;
//         const glm::ivec2& end = *std::next(it);
//         const RoadGraphEdge& edge = RoadGraphEdge(start, end);

//         #if DEBUG
//         std::cout << "Create new edge from: " << edge.start << " to:" << edge.end << std::endl;
//         #endif

//         nodes[start][dir] = edge;
//         nodes[end][-dir] = edge;
//     }
// }

// void RoadGraph::clear() {
//     nodes.clear();
// }

// void RoadGraph::merge(const RoadGraph& other) {
//     for (const auto& [pos, node] : other.nodes) {
//         if (nodes.contains(pos)) {
//             // node exists
//             for (const auto& edge : node.edges) {
//                 if ()
//             }
//         }
//     }
// }

// std::vector<glm::ivec2> RoadGraph::getRoute(const glm::ivec2& start, const glm::ivec2& end) const {
//     struct Node {
//         int distance = INT_MAX;
//         Node* last = nullptr;

//         glm::ivec2 pos;
//     };

//     std::unordered_set<glm::ivec2> unvisitedNodes;
//     for (const auto& [pos, _] : nodes) {
//         unvisitedNodes.emplace(pos);
//     }

//     std::unordered_map<glm::ivec2, Node> nodeDistances;
//     nodeDistances[end] = {0, nullptr, end};

//     Node* currentNode = &nodeDistances[end];
//     Node* lastNode = &nodeDistances[end];

//     // while any nodes are unvisited
//     while (unvisitedNodes.size() > 0) {
//         const glm::ivec2 currentPos = currentNode->pos;
//         if (currentPos == start)
//             break;

//         // node was visited
//         unvisitedNodes.erase(currentPos);

//         const RoadGraphNode& graphNode = nodes.at(currentPos);

//         // calculate distance for unvisited and select node with min distance
//         int minDistance = INT_MAX;
//         Node* nextNode;

//         for (Direction dir = Direction::NORTH; dir != Direction::UNDEFINED; dir++) {
//             if (currentNode != lastNode && graphNode.connected(dir)) {
//                 const glm::ivec2& destination = graphNode.getDestination(dir);

//                 int distanceSum = lastNode->distance + graphNode.edges[(int)dir].length;
//                 if (distanceSum < nodeDistances[destination].distance) {
//                     nodeDistances[destination] = {distanceSum, &nodeDistances[currentPos], destination};

//                     // select unvisited node with minimal distance for the next iteration
//                     if (distanceSum < minDistance && unvisitedNodes.contains(destination)) {
//                         nextNode = &nodeDistances[destination];
//                         minDistance = distanceSum;
//                     }
//                 }
//             }
//         }

//         lastNode = currentNode;
//         currentNode = nextNode;
//     }

//     // iterate through nodes and generate path
//     std::vector<glm::ivec2> path;
//     do {
//         path.push_back(currentNode->pos);

//         currentNode = currentNode->last;
//     } while (currentNode->last != nullptr);

//     return path;
// }
#pragma endregion

RoadGraphEdge::RoadGraphEdge(const glm::ivec2& position1, const glm::ivec2& position2)
    : position1(glm::min(position1, position2)), position2(glm::max(position1, position2)) {
}

std::pair<glm::ivec2, glm::uvec2> RoadGraphEdge::getPosition_Direction() const {
    return std::make_pair(position1, glm::uvec2(position2 - position1));
}

unsigned int RoadGraphEdge::getLength() const {
    return glm::length(position2 - position1);
}

bool RoadGraphEdge::intersect(const RoadGraphEdge& e1, const RoadGraphEdge& e2, glm::ivec2& pos) {
    const auto& [r1, l1] = e1.getPosition_Direction();
    const auto& [r2, l2] = e2.getPosition_Direction();

    if (r1 == r2) {
        pos = r1;
        return true;
    }

    // check if edges are parallel
    const glm::mat2& L{l1, l2};
    if (glm::determinant(L) != 0) {
        // edges not parallel
        const glm::vec2& lambda = glm::inverse(L) * (r2 - r1);

        bool intersection = 0 <= lambda.x && lambda.x <= 1 && 0 <= lambda.y && lambda.y <= 1;
        if (intersection) {
            pos = r1 + static_cast<int>(lambda.x) * glm::ivec2(l1);
        }

        return intersection;
    }

    // edges are parallel
    const glm::mat2& lr{l1, r2 - r1};
    if (glm::determinant(lr) != 0) {
        return false;
    }

    // r1 < r2
    bool intersection = false;
    if (glm::all(glm::lessThanEqual(r1, r2))) {
        intersection = glm::all(glm::lessThanEqual(r2 - r1, glm::ivec2(l1)));

        if (intersection) {
            pos = r2;
        }
    }
    // r1 > r2
    else {
        intersection = glm::all(glm::lessThanEqual(r1 - r2, glm::ivec2(l2)));

        if (intersection) {
            pos = r1;
        }
    }

    return intersection;
}

bool RoadGraph::exists(const glm::ivec2& position) const {
    return connections.contains(position);
}

bool RoadGraph::connected(const glm::ivec2& pos1, const glm::ivec2& pos2) const {
    auto it = connections.find(pos1);

    while (it->second != pos2) {
        it++;
    }

    return it != connections.end();
}

RoadGraphEdge RoadGraph::getEdge(const glm::ivec2& pos1, const glm::ivec2& pos2) const {
    if (connected(pos1, pos2)) {
        return RoadGraphEdge(pos1, pos2);
    }

    throw std::out_of_range("Edge not found");
}

void RoadGraph::addNode(const glm::ivec2& position) {
    if (exists(position)) {
        return;
    }

    connections.insert(std::make_pair(position, position));
}

bool RoadGraph::addEdge(const glm::ivec2& position1, const glm::ivec2& position2, bool bidirectional) {
    if (bidirectional) {
        if (connected(position1, position2) && connected(position1, position2)) {
            return false;
        }

        // TODO: Handle the case that just one direction is connected
    }
    else if (connected(position1, position2)) {
        return false;
    }

    if (position1.x != position2.x && position1.y != position2.y) {
        // edge not valid
        throw std::runtime_error("The edge is not valid!");
    }
    else if (position1 == position2) {
        addNode(position1);
    }

    // Update existing edges
    RoadGraphEdge newEdge{position1, position2};
    std::multimap<glm::ivec2, edgeCollection::iterator> intersections;
    glm::ivec2 intersectionPos;

    auto it = connections.begin();
    while (it != connections.end()) {
        // check intersection
        const auto& [start, end] = *it;
        const RoadGraphEdge& other{start, end};

        bool intersects = RoadGraphEdge::intersect(newEdge, other, intersectionPos);
        if (intersects) {
            intersections.insert(std::make_pair(intersectionPos, it));
        }
    }

    // no intersections
    if (intersections.empty()) {
        connections.insert(std::make_pair(position1, position2));
        if (bidirectional)
            connections.insert(std::make_pair(position2, position1));

        return true;
    }
}
