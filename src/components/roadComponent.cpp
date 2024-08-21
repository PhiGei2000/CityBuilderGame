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
#include "components/roadComponent.hpp"

#include "misc/roads/roadPathGenerator.hpp"

#include "misc/utility.hpp"

RoadComponent::RoadComponent() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < Configuration::cellsPerChunk; j++) {
            borders[i][j] = false;
        }
    }
}

RoadComponent::RoadComponent(const RoadTile (&tiles)[Configuration::cellsPerChunk][Configuration::cellsPerChunk]) {
    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            if (x < 4) {
                this->borders[x][y] = false;
            }

            this->roadTiles[x][y] = tiles[x][y];
        }
    }
}

RoadComponent::RoadComponent(const RoadTile (&tiles)[Configuration::cellsPerChunk][Configuration::cellsPerChunk], const bool (&borders)[4][Configuration::cellsPerChunk]) {
    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            if (x < 4) {
                this->borders[x][y] = borders[x][y];
            }

            this->roadTiles[x][y] = tiles[x][y];
        }
    }
}

void RoadComponent::clear() {
    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            roadTiles[x][y] = RoadTile();
        }

        borders[0][x] = false;
        borders[1][x] = false;
        borders[2][x] = false;
        borders[3][x] = false;
    }
}

void RoadComponent::setRoad(const glm::uvec2& position1, const glm::uvec2& position2) {
    assert(position1.x < Configuration::cellsPerChunk && position1.y < Configuration::cellsPerChunk);
    assert(position2.x < Configuration::cellsPerChunk && position2.y < Configuration::cellsPerChunk);

    const glm::uvec2& min = glm::min(position1, position2);
    const glm::uvec2& max = glm::max(position1, position2);

    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            if (roadTiles[x][y].tileType == RoadTileTypes::EMPTY) {
                roadTiles[y][y].tileType = RoadTileTypes::UNDEFINED;
            }
        }
    }
}

bool RoadComponent::isConnected(const glm::ivec2& pos, Direction dir) const {
    if (dir == Direction::UNDEFINED) {
        // check if the road is connected to any direction
        for (int i = 0; i < 4; i++) {
            if (isConnected(pos, static_cast<Direction>(i))) {
                return true;
            }
        }

        return false;
    }

    const glm::ivec2& direction = DirectionVectors<glm::ivec2>[dir];
    const glm::ivec2& neighbourPos = pos + direction;
    if (neighbourPos.x < 0 || neighbourPos.y < 0 || neighbourPos.x >= Configuration::cellsPerChunk || neighbourPos.t >= Configuration::cellsPerChunk) {
        int directionInt = static_cast<int>(dir);
        int index = directionInt % 2 == 0 ? neighbourPos.y : neighbourPos.x;
        return borders[directionInt][index];
    }

    return roadTiles[neighbourPos.x][neighbourPos.y].tileType != RoadTileTypes::EMPTY;
}

void RoadComponent::updateRoadTypes(const std::map<RoadTypes, RoadSpecs>& specs) {
    // TODO: Maybe add a flag that indicates that the road component was not updated and no update of the connections is neccessary
    // if (!roadsOutdated) {
    //    return;
    // }

    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            updateRoad(glm::ivec2(x, y), specs);
        }
    }
}

void RoadComponent::updateRoad(const glm::ivec2& pos, const std::map<RoadTypes, RoadSpecs>& specs) {
    if (roadTiles[pos.x][pos.y].empty()) {
        return;
    }

    bool connections[4] = {
        pos.x == Configuration::cellsPerChunk - 1 ? borders[static_cast<int>(Direction::NORTH)][pos.y] : roadTiles[pos.x + 1][pos.y].notEmpty(),
        pos.y == Configuration::cellsPerChunk - 1 ? borders[static_cast<int>(Direction::EAST)][pos.x] : roadTiles[pos.x][pos.y + 1].notEmpty(),
        pos.x == 0 ? borders[static_cast<int>(Direction::SOUTH)][pos.y] : roadTiles[pos.x - 1][pos.y].notEmpty(),
        pos.y == 0 ? borders[static_cast<int>(Direction::WEST)][pos.x] : roadTiles[pos.x][pos.y - 1].notEmpty()};

    const RoadTile& tile = getTileType(connections);

    if (tile != roadTiles[pos.x][pos.y]) {
        std::cout << "Set road at " << pos << " type: " << getRoadTileTypeName(tile.tileType) << std::endl;

        roadTiles[pos.x][pos.y].tileType = tile.tileType;
        roadTiles[pos.x][pos.y].rotation = tile.rotation;

        meshOutdated = true;

        if (tile.isRoadNode()) {
            graph.addNode(pos, RoadPathGenerator::generateNodePaths(pos, specs.at(tile.roadType), tile));
        }
        else if (pos.x == 0 || pos.x == Configuration::cellsPerChunk - 1 || pos.y == 0 || pos.y == Configuration::cellsPerChunk - 1) {
            graph.addNode(pos, RoadPathGenerator::generateNodePaths(pos, specs.at(tile.roadType), tile));
        }
        else {
            graph.removeNode(pos);
        }
    }
}

constexpr RoadTile RoadComponent::getTileType(const bool (&connections)[4]) {
    int connectionsCount =
        (connections[0] ? 1 : 0) +
        (connections[1] ? 1 : 0) +
        (connections[2] ? 1 : 0) +
        (connections[3] ? 1 : 0);

    RoadTileTypes type = RoadTileTypes::UNDEFINED;
    int rotation = 0;
    switch (connectionsCount) {
        case 0: // not connected
            type = RoadTileTypes::NOT_CONNECTED;
            break;
        case 1: // end
            type = RoadTileTypes::END;
            if (connections[1])
                rotation = 1;
            else if (connections[2])
                rotation = 2;
            else if (connections[3])
                rotation = 3;
            break;
        case 2: // straight or curve
            // north south
            if (connections[0] && connections[2]) {
                type = RoadTileTypes::STRAIGHT;
            }
            // east west
            else if (connections[1] && connections[3]) {
                type = RoadTileTypes::STRAIGHT;
                rotation = 1;
            }
            // curve
            else {
                type = RoadTileTypes::CURVE;

                int i = 0;
                while (!connections[i])
                    i++;

                if (i != 0) {
                    rotation = i;
                }
                else {
                    rotation = connections[static_cast<int>(Direction::WEST)] ? 3 : 0;
                }
            }
            break;
        case 3: { // t crossing
            type = RoadTileTypes::T_CROSSING;

            int i = 0;
            while (connections[i])
                i++;

            rotation = (i + 1) % 4;
        } break;
        case 4:
            type = RoadTileTypes::CROSSING;
            break;
    }

    return RoadTile{type, rotation};
}

std::unordered_set<glm::ivec2> RoadComponent::getNodes() const {
    // unused
    std::unordered_set<glm::ivec2> positions;

    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            if (roadTiles[x][y].isRoadNode()) {
                positions.emplace(x, y);
            }
        }
    }

    return positions;
}

void RoadComponent::updateRoadGraph(const std::map<RoadTypes, RoadSpecs>& specs) {
    const std::unordered_map<RoadGraph::RoadGraphNode, RoadGraph::NodeData>& nodes = graph.getNodes();
    // identify edges

    std::set<RoadGraph::RoadGraphEdge> edges;
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        for (auto jt = nodes.begin(); jt != it; jt++) {
            const RoadGraph::RoadGraphNode& x = (*it).first;
            const RoadGraph::RoadGraphNode& y = (*jt).first;

            if (x.x == y.x) {
                // horizontal (east-west) edge
                int start = glm::min(x.y, y.y);
                int end = glm::max(x.y, y.y);

                // check if any nodes are between x and y
                // TODO: Check if the road type is changing
                for (int i = start + 1; i < end; i++) {
                    if (roadTiles[x.x][i].empty()) {
                        // nodes x and y are not connected
                        goto nextNodePair;
                    }
                    else if (nodes.contains(glm::ivec2{x.x, i})) {
                        if (graph.adjacent(x, y)) {
                            std::cout << "Removing edge " << x << " -> " << y << "! There is a new node " << glm::ivec2{x.x, i} << " in between" << std::endl;

                            // there is another node beween x and y but the nodes x and y are connected in the graph
                            graph.removeEdge(RoadGraph::RoadGraphEdge(x, y));
                            graph.removeEdge(RoadGraph::RoadGraphEdge(y, x));
                            goto nextNodePair;
                        }
                        else {
                            // there is another node beween x and y and the nodes are connected with roads but not in the graph
                            goto nextNodePair;
                        }
                    }
                }

                graph.addEdge(x, y, RoadPathGenerator::generateEdgePath(RoadGraph::RoadGraphEdge(x, y), specs.at(roadTiles[x.x][x.y].roadType)));
                graph.addEdge(y, x, RoadPathGenerator::generateEdgePath(RoadGraph::RoadGraphEdge(y, x), specs.at(roadTiles[x.x][x.y].roadType)));
            }
            else if (x.y == y.y) {
                // vertical (north-south) edge
                int start = glm::min(x.x, y.x);
                int end = glm::max(x.x, y.x);

                // check if any nodes are between x and y
                for (int i = start + 1; i < end; i++) {
                    if (roadTiles[i][x.y].empty()) {
                        // nodes x and y are not connected
                        goto nextNodePair;
                    }
                    else if (nodes.contains(glm::ivec2(i, x.y))) {
                        if (graph.adjacent(x, y)) {
                            // there is another node beween x and y and the nodes x and y are connected in the graph
                            std::cout << "Removing edge " << x << " -> " << y << "! There is a node " << glm::ivec2{i, x.y} << " in between" << std::endl;

                            graph.removeEdge(RoadGraph::RoadGraphEdge(x, y));
                            graph.removeEdge(RoadGraph::RoadGraphEdge(y, x));
                            goto nextNodePair;
                        }
                        else {
                            // there is another node beween x and y and the nodes are connected with roads but not in the graph
                            goto nextNodePair;
                        }
                    }
                }

                graph.addEdge(x, y, RoadPathGenerator::generateEdgePath(RoadGraph::RoadGraphEdge(x, y), specs.at(roadTiles[x.x][x.y].roadType)));
                graph.addEdge(y, x, RoadPathGenerator::generateEdgePath(RoadGraph::RoadGraphEdge(y, x), specs.at(roadTiles[x.x][x.y].roadType)));
            }

        nextNodePair:
            continue;
        }
    }

    for (const auto& edge : graph.getEdges()) {
        const auto& [x, y] = edge.first;
        std::cout << "edge: " << x << " -> " << y << std::endl;
    }
}
