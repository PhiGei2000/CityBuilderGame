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
#include "systems/environmentSystem.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "resources/resourceManager.hpp"

#include "components/components.hpp"
#include "events/events.hpp"
#include "misc/configuration.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/utility.hpp"

#include <glm/glm.hpp>

#include <format>

EnvironmentSystem::EnvironmentSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<BuildEvent>()
        .connect<&EnvironmentSystem::handleBuildEvent>(*this);

    eventDispatcher.sink<ChunkCreatedEvent>()
        .connect<&EnvironmentSystem::handleChunkCreatedEvent>(*this);
}

void EnvironmentSystem::init() {

    constexpr float sunAngle = 0.0f;
    registry.emplace<SunLightComponent>(game->sun,
                                        sunAngle,    // direction
                                        sunLight[0], // ambient
                                        sunLight[1], // diffuse
                                        sunLight[2]  // specular
    );
    registry.emplace<TransformationComponent>(game->sun,
                                              utility::sphericalToCartesian(300.0f, sunAngle, 0.0f),
                                              glm::quat(glm::cos(sunAngle / 2), 0, 0, glm::sin(sunAngle / 2)),
                                              glm::vec3(50.0f));
    registry.emplace<MeshComponent>(game->sun, resourceManager.getResource<Mesh<>>("SUN_MESH"));

    EntityMoveEvent moveEvent{game->sun};
    game->raiseEvent(moveEvent);
}

void EnvironmentSystem::updateDayNightCycle(float dt, TransformationComponent& sunTransform, SunLightComponent& sun) const {
    // sun movement
    constexpr float sunSpeed = 2 * glm::pi<float>() / 600.0f;
    // constexpr float sunSpeed = 0.1f;
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(game->camera);

    // move sun
    static constexpr float two_pi = 2 * glm::pi<float>();
    sun.angle += sunSpeed * dt;
    if (sun.angle > two_pi) {
        sun.angle -= two_pi;
    }
    else if (sun.angle < -two_pi) {
        sun.angle += two_pi;
    }

    // intensity
    if (utility::inRange(sun.angle, 0.1f * glm::pi<float>(), 0.9f * glm::pi<float>())) {
        sun.diffuse = glm::sin((sun.angle - 0.1f * glm::pi<float>()) / 0.8f) * sunLight[1];
        sun.specular = glm::sin((sun.angle - 0.1f * glm::pi<float>()) / 0.8f) * sunLight[2];
    }
    else {
        sun.diffuse = glm::vec3(0);
        sun.specular = glm::vec3(0);
    }

    sun.direction = -glm::vec3(glm::cos(sun.angle), glm::sin(sun.angle), 0.0f);
    sunTransform.position = -300.0f * sun.direction + glm::vec3(cameraTransform.position.x, 0.0f, cameraTransform.position.z);
    sunTransform.rotation = glm::quat(glm::cos(sun.angle / 2), 0, 0, glm::sin(sun.angle / 2));
    sunTransform.calculateTransform();

    EntityMoveEvent e{game->sun};
    game->raiseEvent(e);
}

void EnvironmentSystem::update(float dt) {
    // TODO: Optimize this
    while (cellsToClear.size() > 0) {
        const glm::ivec2& position = cellsToClear.front();
        registry.view<EnvironmentComponent, InstancedMeshComponent, TransformationComponent>()
            .each([&](const EnvironmentComponent& environment, InstancedMeshComponent& instancedMesh, const TransformationComponent& transformation) {
                auto it = instancedMesh.transformations.begin();
                bool needsUpdate = false;

                while (it != instancedMesh.transformations.end()) {
                    const glm::vec3& objectPosition = (*it).position;
                    const glm::ivec2 gridPosition = glm::floor(utility::worldToNormalizedWorldGridCoords(objectPosition));

                    if (gridPosition == position) {
                        it = instancedMesh.transformations.erase(it);
                        needsUpdate = true;
                    }
                    else {
                        it++;
                    }
                }

                instancedMesh.instanceBuffer.fillBuffer(instancedMesh.transformations);
            });
        cellsToClear.pop();
    }

    while (entitiesToDestroy.size() > 0) {
        auto entity = entitiesToDestroy.front();

        if (registry.valid(entity)) {
            registry.destroy(entity);
        }

        entitiesToDestroy.pop();
    }
    // return;

    auto& sunLight = registry.get<SunLightComponent>(game->sun);
    auto& sunTransform = registry.get<TransformationComponent>(game->sun);

    updateDayNightCycle(dt, sunTransform, sunLight);
}

void EnvironmentSystem::handleBuildEvent(const BuildEvent& e) {
    if (e.action == BuildAction::END) {
        switch (e.shape) {
            case BuildShape::POINT:
                cellsToClear.emplace(e.positions[0]);
                break;
            case BuildShape::LINE: {
                int segmentsCount = e.positions.size() - 1;
                for (int i = 0; i < segmentsCount; i++) {
                    const glm::ivec2 direction = glm::normalize(e.positions[i + 1] - e.positions[i]);
                    glm::ivec2 current = e.positions[i];

                    while (current != e.positions[i + 1]) {
                        cellsToClear.push(current);
                        current += direction;
                    }
                }
            } break;
            case BuildShape::AREA: {
            } break;
        }
    }
}

void EnvironmentSystem::handleChunkCreatedEvent(const ChunkCreatedEvent& e) const {
    MeshPtr treeMesh = resourceManager.getResource<Mesh<>>("TREE_MESH");
    const TerrainComponent& terrain = registry.get<TerrainComponent>(e.entity);
    std::unordered_map<std::string, InstancedMesh<TransformationComponent>> transformations;

    const std::array<std::string, 2> treeNames = {"tree01", "tree02"};

    // spawn trees
    for (int i = 0; i < 100; i++) {
        glm::vec2 chunkGridPos = Configuration::cellsPerChunk / static_cast<float>(RAND_MAX) * glm::vec2(rand(), rand());
        const glm::vec2& gridPos = utility::normalizedChunkGridToNormalizedWorldGridCoords(e.chunkPosition, chunkGridPos);

        TerrainSurfaceTypes surfaceType = game->terrain.getSurfaceType(gridPos);
        if (surfaceType == TerrainSurfaceTypes::GRASS) {
            glm::vec3 position = glm::vec3(Configuration::cellSize * chunkGridPos.x, game->terrain.getTerrainHeight(gridPos), Configuration::cellSize * chunkGridPos.y);
            float angle = (float)rand() / static_cast<float>(RAND_MAX) * 0.5f * glm::pi<float>();
            glm::vec3 scale = glm::vec3((float)rand() / static_cast<float>(RAND_MAX) * 0.5 + 1.5f);
            int type = rand() % 2;

            transformations[treeNames[type]].transformations.emplace_back(position, glm::quat(glm::vec3(0, angle, 0)), scale);
        }
    }

    for (auto& [name, instancedMesh] : transformations) {
        instancedMesh.instanceBuffer.fillBuffer(instancedMesh.transformations);
    }

    // entt::entity entity = registry.create();
    MultiInstancedMeshComponent& instancedMesh = registry.emplace<MultiInstancedMeshComponent>(e.entity, treeMesh, transformations);
    // registry.emplace<TransformationComponent>(entity, static_cast<float>(Configuration::chunkSize) * glm::vec3(e.chunkPosition.x, 0.0f, e.chunkPosition.y), glm::quat(), glm::vec3(1.0f));
    registry.emplace<EnvironmentComponent>(e.entity);
}
