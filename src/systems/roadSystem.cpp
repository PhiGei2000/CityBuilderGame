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

    for (const auto& [buildingID, roadPack] : roadTypes) {
        roadSpecs[buildingID] = roadPack->specs;
    }
}

void RoadSystem::update(float dt) {
    while (!chunksToUpdateMesh.empty()) {
        const glm::ivec2& chunkPos = chunksToUpdateMesh.front();
        const entt::entity chunk = game->terrain.chunkEntities.at(chunkPos);

        const auto& [road, roadMesh] = registry.get<RoadComponent, MeshComponent>(chunk);
        createRoadMesh(road, roadMesh, chunkPos);
        chunksToUpdateMesh.pop();
    }

    while (!roadsToBuild.empty()) {
        const auto& [pos, type] = roadsToBuild.front();

        const auto& [chunk, chunkPos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(pos);
        glm::ivec2 lastChunk = chunk;
        entt::entity chunkEntity = game->terrain.chunkEntities.at(chunk);

        RoadComponent& road = registry.get<RoadComponent>(chunkEntity);
        road.roadTiles[chunkPos.x][chunkPos.y].tileType = RoadTileTypes::UNDEFINED;
        road.roadTiles[chunkPos.x][chunkPos.y].roadType = type;

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
        adjustTerrainHeight(chunk, chunkPos, road);

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

void RoadSystem::createRoadMesh(const RoadComponent& road, MeshComponent& mesh, const glm::ivec2& chunkPos) const {
    std::map<std::string, std::map<RoadTileTypes, std::vector<glm::mat4>>> transforms;

    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            const RoadTile& tile = road.roadTiles[x][y];
            RoadTileTypes tileType = tile.tileType;

            if (tile.notEmpty()) {
                float roadHeight = getRoadHeight(chunkPos * Configuration::cellsPerChunk + glm::ivec2(x, y), tile);

                const glm::vec3& pos = static_cast<float>(Configuration::cellSize) * glm::vec3(x + 0.5f, 0.0f, y + 0.5f) + glm::vec3(0.0f, roadHeight, 0.0f);

                transforms[tile.roadType][tileType].emplace_back(tile.getTransform(pos));
            }
        }
    }

    for (const auto& [type, roadPack] : resourceManager.getResources<RoadPack>()) {
        for (RoadTileTypes tileType = RoadTileTypes::NOT_CONNECTED; tileType <= RoadTileTypes::RAMP; tileType++) {
            const std::string& tileTypeName = getRoadTileTypeName(tileType);
            // const std::string& roadMeshId = type + "." + tileTypeName;

            bool updateBuffer = mesh.instancedMeshes.contains(type);
            if (updateBuffer) {
                updateBuffer |= mesh.instancedMeshes[type].instances.contains(tileTypeName);
            }
            else if (transforms.size() > 0) {
                mesh.instancedMeshes[type] = InstancedMesh<glm::mat4>{roadPack->roadGeometries};
            }

            if (transforms.contains(type)) {
                updateBuffer |= transforms[type].contains(tileType);
            }

            if (updateBuffer) {
                const std::vector<glm::mat4>& tiles = transforms[type][tileType];
                if (tiles.size() > 0) {
                    mesh.instancedMeshes[type].instances[tileTypeName].instanceBuffer.fillBuffer(tiles);
                }
                else {
                    mesh.instancedMeshes[type].instances[tileTypeName].instanceBuffer.clearBuffer();
                }
            }
        }
    }

#if DEBUG
    // std::vector<float> positions;
    // std::vector<unsigned int> indicesLines;
    // unsigned int currentIndex = 0;

    // for (const auto& [node, nodeData] : road.graph.getNodes()) {
    //     const auto& paths = nodeData.paths;

    //     for (int i = 0; i < 4; i++) {
    //         for (int j = 0; j < 4; j++) {
    //             for (int k = 0; k < paths[i][j].length(); k++) {
    //                 positions.insert(positions.end(), {paths[i][j][k].x, paths[i][j][k].y, paths[i][j][k].z});

    //                 if (k > 0) {
    //                     indicesLines.push_back(currentIndex);
    //                 }

    //                 if (k < paths[i][j].length() - 1) {
    //                     indicesLines.push_back(currentIndex);
    //                 }

    //                 currentIndex++;
    //             }
    //         }
    //     }
    // }

    // for (const auto& [edge, path] : road.graph.getEdges()) {
    //     unsigned int pathLength = path.length();

    //     for (int i = 0; i < pathLength; i++) {
    //         positions.insert(positions.end(), {path[i].x, path[i].y, path[i].z});
    //         indicesLines.push_back(currentIndex);

    //         currentIndex++;
    //     }
    // }

    // mesh.mesh->geometries["roadGraphDebug"]->bufferData(positions, indicesLines, GL_STATIC_DRAW);
#endif
}

void RoadSystem::adjustTerrainHeight(const glm::ivec2& chunkPos, const glm::ivec2& position, RoadComponent& road) const {
    const glm::ivec2& cellPos = Configuration::cellsPerChunk * chunkPos + position;

    const std::array<float, 4>& cellHeights = game->terrain.getTerrainCellHeights(cellPos);
    float roadHeight = cellHeights[0];
    bool flatCell = true;

    for (int i = 1; i < 4; i++) {
        if (cellHeights[i] != cellHeights[0]) {
            flatCell = false;

            roadHeight = std::max(roadHeight, cellHeights[i]);
        }
    }

    RoadTile& tile = road.roadTiles[position.x][position.y];
    // adjust terrain or build a ramp
    if (!flatCell) {
        if (tile.tileType == RoadTileTypes::STRAIGHT) {
            switch (road.roadTiles[position.x][position.y].rotation % 2) {
                // north - south
                case 0: {
                    float h0 = game->terrain.getTerrainHeight(cellPos + glm::ivec2(-1, 0));
                    float h1 = game->terrain.getTerrainHeight(cellPos + glm::ivec2(1, 0));

                    game->terrain.setTerrainCellHeights(cellPos, {h0, h1, h0, h1});
                    tile.tileType = RoadTileTypes::RAMP;
                    tile.rotation = h0 > h1 ? 2 : 0;
                } break;

                // east - west
                case 1: {
                    float h0 = game->terrain.getTerrainHeight(cellPos + glm::ivec2(0, -1));
                    float h1 = game->terrain.getTerrainHeight(cellPos + glm::ivec2(0, 1));

                    game->terrain.setTerrainCellHeights(cellPos, {h0, h0, h1, h1});
                    tile.tileType = RoadTileTypes::RAMP;
                    tile.rotation = h0 > h1 ? 3 : 1;
                } break;
                default:
                    break;
            }
        }
        else {
            game->terrain.setTerrainCellHeights(cellPos, {roadHeight, roadHeight, roadHeight, roadHeight});
        }
    }
}

float RoadSystem::getRoadHeight(const glm::ivec2& position, const RoadTile& road) const {
    const std::array<float, 4> cellHeights = game->terrain.getTerrainCellHeights(position);

    if (road.tileType == RoadTileTypes::RAMP) {
        return *std::min_element(cellHeights.begin(), cellHeights.end());
    }

    return *std::max_element(cellHeights.begin(), cellHeights.end());
}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    if (!event.buildingID.starts_with("infrastructure.road"))
        return;

    roadsToBuild.emplace(event.positions[0], event.buildingID);

    // destroy entity
    registry.destroy(event.entity);
}
