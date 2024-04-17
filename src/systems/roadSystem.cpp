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
#include "misc/roads/roadTypes.hpp"
#include "misc/utility.hpp"
#include "resources/roadPack.hpp"

RoadSystem::RoadSystem(Game* game)
    : System(game) {

    eventDispatcher.sink<BuildEvent>()
        .connect<&RoadSystem::handleBuildEvent>(*this);

    init();
}

void RoadSystem::update(float dt) {
    RoadPackPtr roadPack = resourceManager.getResource<RoadPack>("BASIC_STREETS");

    while (!chunksToUpdateMesh.empty()) {
        const entt::entity chunk = game->terrain.chunkEntities.at(chunksToUpdateMesh.front());

        const auto& [road, roadMesh] = registry.get<RoadComponent, RoadMeshComponent>(chunk);
        createRoadMesh(road, roadMesh);
        chunksToUpdateMesh.pop();
    }

    while (!roadsToBuild.empty()) {
        const auto& [start, end] = roadsToBuild.front();
        // const glm::ivec2& dir = DirectionVectors[utility::getDirection(end - start)];

        glm::ivec2 pos = start;
        const auto& [chunk, chunkPos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(pos);
        glm::ivec2 lastChunk = chunk;
        entt::entity chunkEntity = game->terrain.chunkEntities.at(chunk);

        // do {
        RoadComponent& road = registry.get<RoadComponent>(chunkEntity);
        road.roadTiles[chunkPos.x][chunkPos.y].tileType = RoadTileTypes::UNDEFINED;

        road.updateRoadTypes();
        chunksToUpdateMesh.push(chunk);
        // } while (pos != end);
        roadsToBuild.pop();
    }
}

void RoadSystem::createRoadMesh(const RoadComponent& road, RoadMeshComponent& geometry) const {
    std::map<RoadTypes, std::map<RoadTileTypes, std::vector<glm::mat4>>> transforms;

    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            const RoadTile& tile = road.roadTiles[x][y];
            if (tile.notEmpty()) {
                const glm::vec3& pos = static_cast<float>(Configuration::cellSize) * glm::vec3(x + 0.5f, 0, y + 0.5f);
                float cos = glm::cos(glm::radians(90.0f * tile.rotation));
                float sin = glm::sin(glm::radians(90.0f * tile.rotation));

                transforms[tile.roadType][tile.tileType].emplace_back(glm::vec4(cos, 0.0f, sin, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(-sin, 0.0f, cos, 0.0f), glm::vec4(pos, 1.0f));
            }
        }
    }

    for (RoadTypes type = RoadTypes::BASIC_STREETS; type < RoadTypes::UNDEFINED; type++) {
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
}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    if (event.type != BuildingType::ROAD || event.action != BuildAction::END)
        return;

    roadsToBuild.emplace(event.positions[0], event.positions[0]);

    // destroy entity
    registry.destroy(event.entity);
}
