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
#include "systems/roadSystem.hpp"

#include "components/components.hpp"
#include "events/buildEvent.hpp"
#include "misc/configuration.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/direction.hpp"
#include "misc/roads/roadPathGenerator.hpp"
#include "misc/roads/roadTypes.hpp"
#include "misc/utility.hpp"
#include "resources/roadPack.hpp"

#if DEBUG
#include <GL/gl.h>
#endif

RoadSystem::RoadSystem(Game* game)
    : System(game) {

    eventDispatcher.sink<BuildEvent>()
        .connect<&RoadSystem::handleBuildEvent>(*this);

    init();
}

void RoadSystem::init() {
    const auto& roadTypes = resourceManager.getResources<RoadPack>();

    roadSpecs[RoadTypes::BASIC_ROADS] = roadTypes.at("BASIC_ROADS")->specs;
}

void RoadSystem::update(float dt) {
    RoadPackPtr roadPack = resourceManager.getResource<RoadPack>("BASIC_ROADS");

    while (!chunksToUpdateMesh.empty()) {
        const entt::entity chunk = game->terrain.chunkEntities.at(chunksToUpdateMesh.front());

        const auto& [road, roadMesh] = registry.get<RoadComponent, RoadMeshComponent>(chunk);
        createRoadMesh(road, roadMesh);
        chunksToUpdateMesh.pop();
    }

    while (!roadsToBuild.empty()) {
        const glm::ivec2& pos = roadsToBuild.front();

        const auto& [chunk, chunkPos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(pos);
        glm::ivec2 lastChunk = chunk;
        entt::entity chunkEntity = game->terrain.chunkEntities.at(chunk);

        RoadComponent& road = registry.get<RoadComponent>(chunkEntity);
        road.roadTiles[chunkPos.x][chunkPos.y].tileType = RoadTileTypes::UNDEFINED;

        // update borders
        Direction dir = Direction::UNDEFINED;
        int borderPos = -1;

        if (chunkPos.x == Configuration::cellsPerChunk - 1) {
            dir = Direction::NORTH;
            borderPos = chunkPos.y;
        }
        else if (chunkPos.y == Configuration::cellsPerChunk - 1) {
            dir = Direction::EAST;
            borderPos = chunkPos.x;
        }
        else if (chunkPos.x == 0) {
            dir = Direction::SOUTH;
            borderPos = chunkPos.y;
        }
        else if (chunkPos.y == 0) {
            dir = Direction::WEST;
            borderPos = chunkPos.x;
        }

        if (dir != Direction::UNDEFINED) {
            glm::ivec2 neighbourChunk = chunk + DirectionVectors<glm::ivec2>[dir];
            if (game->terrain.chunkLoaded(neighbourChunk)) {
                RoadComponent& neighbourRoads = registry.get<RoadComponent>(game->terrain.chunkEntities[neighbourChunk]);

                neighbourRoads.borders[static_cast<int>(utility::getInverse(dir))][borderPos] = true;
                glm::ivec2 posInNeighbourChunk = chunkPos - (Configuration::cellsPerChunk - 1) * DirectionVectors<glm::ivec2>[dir];
                neighbourRoads.updateRoad(posInNeighbourChunk, roadSpecs);

                if (neighbourRoads.meshOutdated) {
                    chunksToUpdateMesh.push(neighbourChunk);
                }
            }
        }

        road.updateRoad(chunkPos, roadSpecs);

        // update neighbour roads
        for (unsigned int i = 0; i < 4; i++) {
            const glm::ivec2& pos = chunkPos - DirectionVectors<glm::ivec2>[static_cast<Direction>(i)];
            if (utility::inChunk(pos)) {
                road.updateRoad(pos, roadSpecs);
            }
        }

        road.updateRoadGraph(roadSpecs);
        if (road.meshOutdated) {
            chunksToUpdateMesh.push(chunk);
        }
        roadsToBuild.pop();
    }
}

void RoadSystem::createRoadMesh(const RoadComponent& road, RoadMeshComponent& geometry) const {
    std::map<RoadTypes, std::map<RoadTileTypes, std::vector<glm::mat4>>> transforms;
    constexpr int sinValues[] = {0, 1, 0, -1};
    constexpr int cosValues[] = {1, 0, -1, 0};

    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            const RoadTile& tile = road.roadTiles[x][y];
            if (tile.notEmpty()) {
                const glm::vec3& pos = static_cast<float>(Configuration::cellSize) * glm::vec3(x + 0.5f, 0, y + 0.5f);
                float cos = cosValues[tile.rotation];
                float sin = sinValues[tile.rotation];

                transforms[tile.roadType][tile.tileType].emplace_back(glm::vec4(cos, 0.0f, sin, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(-sin, 0.0f, cos, 0.0f), glm::vec4(pos, 1.0f));
            }
        }
    }

    for (RoadTypes type = RoadTypes::BASIC_ROADS; type < RoadTypes::UNDEFINED; type++) {
        for (RoadTileTypes tileType = RoadTileTypes::NOT_CONNECTED; tileType < RoadTileTypes::CURVE_FULL; tileType++) {
            bool updateBuffer = false;
            if (geometry.roadMeshes.contains(type)) {
                updateBuffer |= geometry.roadMeshes[type].contains(tileType);
            }

            if (transforms.contains(type)) {
                updateBuffer |= transforms[type].contains(tileType);
            }

            if (updateBuffer) {
                const std::vector<glm::mat4>& tiles = transforms[type][tileType];
                if (tiles.size() > 0) {
                    geometry.roadMeshes[type][tileType].instanceBuffer.fillBuffer(tiles);
                }
                else {
                    geometry.roadMeshes[type][tileType].instanceBuffer.clearBuffer();
                }
            }
        }
    }

#if DEBUG
    std::vector<float> positions;
    std::vector<unsigned int> indicesLines;
    unsigned int currentIndex = 0;

    RoadPackPtr basicRoads = resourceManager.getResource<RoadPack>("BASIC_ROADS");
    for (const auto& [node, paths] : road.graph.getNodes()) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < paths[i][j].size(); k++) {
                    positions.insert(positions.end(), {paths[i][j][k].x, paths[i][j][k].y, paths[i][j][k].z});

                    if (k > 0) {
                        indicesLines.push_back(currentIndex);
                    }

                    if (k < paths[i][j].size() - 1) {
                        indicesLines.push_back(currentIndex);
                    }

                    currentIndex++;
                }
            }
        }
    }

    for (const auto& [edge, path] : road.graph.getEdges()) {
        unsigned int pathLength = path.size();

        for (int i = 0; i < pathLength; i++) {
            positions.insert(positions.end(), {path[i].x, path[i].y, path[i].z});
            indicesLines.push_back(currentIndex);

            currentIndex++;
        }
    }

    geometry.graphDebugMesh->bufferData(positions, indicesLines, GL_STATIC_DRAW);
#endif
}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    if (event.type != BuildingType::ROAD || event.action != BuildAction::END)
        return;

    roadsToBuild.emplace(event.positions[0]);

    // destroy entity
    registry.destroy(event.entity);
}
