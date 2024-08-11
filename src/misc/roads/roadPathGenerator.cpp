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

#include "misc/roads/roadPathGenerator.hpp"

#include "misc/roads/roadGraph.hpp"

#include "misc/configuration.hpp"
#include "misc/direction.hpp"
#include "misc/utility.hpp"

#include <glm/gtc/constants.hpp>

RoadPath RoadPathGenerator::generateEdgePath(const RoadGraph::RoadGraphEdge& edge, const RoadSpecs& specs) {
    const auto& [start, end] = edge;
    if (glm::length(end - start) <= 1) {
        return {};
    }

    Direction direction = utility::getDirection(end - start);
    glm::vec3 offset = glm::vec3(0.0f, specs.roadwayHeight, 0.0f);

    switch (direction) {
        case Direction::NORTH:
            offset.x += Configuration::cellSize;
            offset.z += Configuration::cellSize * (0.5f + specs.roadwayWidth / 4.0f);
            break;
        case Direction::EAST:
            offset.x += Configuration::cellSize * (0.5f - specs.roadwayWidth / 4.0f);
            offset.z += Configuration::cellSize;
            break;
        case Direction::SOUTH:
            offset.z += Configuration::cellSize * (0.5f - specs.roadwayWidth / 4.0f);
            break;
        case Direction::WEST:
            offset.x += Configuration::cellSize * (0.5f + specs.roadwayWidth / 4.0f);
            break;
        default:
            break;
    }

    glm::ivec2 directionVector = DirectionVectors<glm::ivec2>[direction];
    const glm::vec3& pathBegin = static_cast<float>(Configuration::cellSize) * glm::vec3(start.x, 0.0f, start.y) + offset;
    const glm::vec3& pathEnd = static_cast<float>(Configuration::cellSize) * glm::vec3(end.x - directionVector.x, 0.0f, end.y - directionVector.y) + offset;

    return {pathBegin, pathEnd};
}

std::array<std::array<RoadPath, 4>, 4> RoadPathGenerator::generateNodePaths(const RoadGraph::RoadGraphNode& node, const RoadSpecs& specs, const RoadTile& tile) {
    std::array<std::array<RoadPath, 4>, 4> paths;

    constexpr int sinValues[] = {0, 1, 0, -1};
    constexpr int cosValues[] = {1, 0, -1, 0};

    float cos = cosValues[tile.rotation];
    float sin = sinValues[tile.rotation];
    // glm::mat3 rotationMatrix = glm::mat3{
    //     {cos, 0, -sin},
    //     {  0, 1,    0},
    //     {sin, 0,  cos}
    // };

    auto addToPath = [&](int i, int j, float x, float z) {
        paths[i % 4][j % 4].push_back(static_cast<float>(Configuration::cellSize) * glm::vec3(cos * x - sin * z + 0.5f + node.x, 0.0f, sin * x + cos * z + 0.5f + node.y) + glm::vec3(0.0f, specs.roadwayHeight, 0.0f));
    };

    float anglePerPoint = 2 * glm::pi<float>() / specs.verticesPerCircle;
    float innerRadius = specs.roadwayWidth / 4.0f;
    switch (tile.tileType) {
        case RoadTileTypes::END:
            addToPath(tile.rotation, tile.rotation, 0.5f, -specs.roadwayWidth / 4.0f);

            for (int i = 0; i < specs.verticesPerCircle / 2; i++) {
                float x = -innerRadius * glm::sin(i * anglePerPoint);
                float z = -innerRadius * glm::cos(i * anglePerPoint);

                addToPath(tile.rotation, tile.rotation, x, z);
            }

            addToPath(tile.rotation, tile.rotation, 0.0f, specs.roadwayWidth / 4.0f);
            addToPath(tile.rotation, tile.rotation, 0.5f, specs.roadwayWidth / 4.0f);
            break;
        case RoadTileTypes::NOT_CONNECTED:
            for (int i = 0; i < specs.verticesPerCircle; i++) {
                float x = innerRadius * glm::cos(i * anglePerPoint);
                float z = innerRadius * glm::sin(i * anglePerPoint);

                addToPath(0, 0, x, z);
            }

            addToPath(0, 0, innerRadius, 0);
            break;
        case RoadTileTypes::STRAIGHT:
            addToPath(tile.rotation, tile.rotation + 2, 0.5f, -specs.roadwayWidth / 4.0f);
            addToPath(tile.rotation, tile.rotation + 2, -0.5f, -specs.roadwayWidth / 4.0f);

            addToPath(tile.rotation + 2, tile.rotation, -0.5f, specs.roadwayWidth / 4.0f);
            addToPath(tile.rotation + 2, tile.rotation, 0.5f, specs.roadwayWidth / 4.0f);
            break;
        case RoadTileTypes::CURVE:
        case RoadTileTypes::CURVE_FULL:
            for (int i = 0; i <= specs.verticesPerCircle / 4; i++) {
                float sin = glm::sin(i * anglePerPoint);
                float cos = glm::cos(i * anglePerPoint);

                // inner path
                addToPath(tile.rotation + 1, tile.rotation, 0.5f - (0.5f - innerRadius) * cos, 0.5f - (0.5f - innerRadius) * sin);
                // outer path
                addToPath(tile.rotation, tile.rotation + 1, 0.5f - (0.5f + innerRadius) * sin, 0.5f - (0.5f + innerRadius) * cos);
            }
            break;
        case RoadTileTypes::T_CROSSING:
            // straight paths
            addToPath(tile.rotation, tile.rotation + 2, 0.5f, -innerRadius);
            addToPath(tile.rotation, tile.rotation + 2, -0.5f, -innerRadius);

            addToPath(tile.rotation + 2, tile.rotation, -0.5f, innerRadius);
            addToPath(tile.rotation + 2, tile.rotation, 0.5f, innerRadius);

            for (int i = 0; i <= specs.verticesPerCircle / 4; i++) {
                float sin = glm::sin(i * anglePerPoint);
                float cos = glm::cos(i * anglePerPoint);

                // inner paths
                addToPath(tile.rotation + 1, tile.rotation, 0.5f - (0.5f - innerRadius) * cos, 0.5f - (0.5f - innerRadius) * sin);
                addToPath(tile.rotation + 2, tile.rotation + 1, -0.5f + (0.5f - innerRadius) * sin, 0.5f - (0.5f - innerRadius) * cos);
                // outer paths
                addToPath(tile.rotation, tile.rotation + 1, 0.5f - (0.5f + innerRadius) * sin, 0.5f - (0.5f + innerRadius) * cos);
                addToPath(tile.rotation + 1, tile.rotation + 2, -0.5f + (0.5f + innerRadius) * cos, 0.5f - (0.5f + innerRadius) * sin);
            }
            break;
        case RoadTileTypes::CROSSING:
            // straight paths
            // N -> S
            addToPath(0, 2, 0.5f, -innerRadius);
            addToPath(0, 2, -0.5f, -innerRadius);

            // // E -> W
            addToPath(1, 3, innerRadius, 0.5f);
            addToPath(1, 3, innerRadius, -0.5f);

            // // S -> N
            addToPath(2, 0, -0.5f, innerRadius);
            addToPath(2, 0, 0.5f, innerRadius);

            // // W -> E
            addToPath(1, 3, -innerRadius, -0.5f);
            addToPath(1, 3, -innerRadius, 0.5f);

            for (int i = 0; i <= specs.verticesPerCircle / 4; i++) {
                float sin = glm::sin(i * anglePerPoint);
                float cos = glm::cos(i * anglePerPoint);

                // inner paths
                addToPath(1, 0, 0.5f - (0.5f - innerRadius) * cos, 0.5f - (0.5f - innerRadius) * sin);
                addToPath(2, 1, -0.5f + (0.5f - innerRadius) * sin, 0.5f - (0.5f - innerRadius) * cos);
                addToPath(3, 2, -0.5f + (0.5f - innerRadius) * cos, -0.5f + (0.5f - innerRadius) * sin);
                addToPath(0, 3, 0.5f - (0.5f - innerRadius) * sin, -0.5f + (0.5f - innerRadius) * cos);
                // outer paths
                addToPath(0, 1, 0.5f - (0.5f + innerRadius) * sin, 0.5f - (0.5f + innerRadius) * cos);
                addToPath(1, 2, -0.5f + (0.5f + innerRadius) * cos, 0.5f - (0.5f + innerRadius) * sin);
                addToPath(2, 3, -0.5f + (0.5f + innerRadius) * sin, -0.5f + (0.5f + innerRadius) * cos);
                addToPath(3, 0, 0.5f - (0.5f + innerRadius) * cos, -0.5f + (0.5f + innerRadius) * sin);
            }
        default:
            break;
    }

    return paths;
}