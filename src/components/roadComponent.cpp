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

#include "misc/utility.hpp"

RoadComponent::RoadComponent() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < Configuration::cellsPerChunk; j++) {
            borders[i][j] = false;
        }
    }
}

RoadComponent::RoadComponent(const RoadTile (&tiles)[Configuration::cellsPerChunk][Configuration::cellsPerChunk]) {
    for (int x =0 ; x < Configuration::cellsPerChunk;x++) {
        for (int y =0 ; y < Configuration::cellsPerChunk; y++) {
            if (x < 4) {
                this->borders[x][y] = false;
            }

            this->roadTiles[x][y] = tiles[x][y];
        }
    }
}

RoadComponent::RoadComponent(const RoadTile (&tiles)[Configuration::cellsPerChunk][Configuration::cellsPerChunk], const bool (&borders)[4][Configuration::cellsPerChunk]) {
    for (int x =0 ; x < Configuration::cellsPerChunk;x++) {
        for (int y =0 ; y < Configuration::cellsPerChunk; y++) {
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
                roadTiles[y][y].tileType == RoadTileTypes::UNDEFINED;
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

    const glm::ivec2& direction = DirectionVectors[dir];
    const glm::ivec2& neighbourPos = pos + direction;
    if (neighbourPos.x < 0 || neighbourPos.y < 0 || neighbourPos.x >= Configuration::cellsPerChunk || neighbourPos.t >= Configuration::cellsPerChunk) {
        int directionInt = static_cast<int>(dir);
        int index = directionInt % 2 == 0 ? neighbourPos.y : neighbourPos.x;
        return borders[directionInt][index];
    }

    return roadTiles[neighbourPos.x][neighbourPos.y].tileType != RoadTileTypes::EMPTY;
}

void RoadComponent::updateRoadTypes() {
    // TODO: Maybe add a flag that indicates that the road component was not updated and no update of the connections is neccessary
    // if (!roadsOutdated) {
    //    return;
    // }

    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            bool connections[4] = {
                x == Configuration::cellsPerChunk - 1 ? borders[static_cast<int>(Direction::NORTH)][y] : roadTiles[x + 1][y].notEmpty(),
                y == Configuration::cellsPerChunk - 1 ? borders[static_cast<int>(Direction::EAST)][x] : roadTiles[x][y + 1].notEmpty(),
                x == 0 ? borders[static_cast<int>(Direction::SOUTH)][y] : roadTiles[x - 1][y].notEmpty(),
                y == 0 ? borders[static_cast<int>(Direction::WEST)][y] : roadTiles[x][y - 1].notEmpty()};

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
                        type = RoadTileTypes::CURVE_FULL;

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

                    rotation = i - 3;
                } break;
                case 4:
                    type = RoadTileTypes::CROSSING;
                    break;
            }

            roadTiles[x][y].tileType = type;
            roadTiles[x][y].rotation = rotation;
        }
    }

    meshOutdated = true;
}

std::vector<glm::ivec2> RoadComponent::getNodes() const {
    std::vector<glm::ivec2> positions;

    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            if (roadTiles[x][y].isRoadNode()) {
                positions.emplace_back(x, y);
            }
        }
    }

    return positions;
}
