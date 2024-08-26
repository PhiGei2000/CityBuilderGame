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
#include <glm/gtx/transform.hpp>

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

    constexpr float sunAngle = glm::radians(30.0f);
    registry.emplace<SunLightComponent>(game->sun,
                                        sunAngle,         // direction
                                        sunLight.ambient, // ambient
                                        sunLight.diffuse, // diffuse
                                        sunLight.specular // specular
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
    // constexpr float sunSpeed = 2 * glm::pi<float>() / 600.0f; // one day lasts 10 minutes
    constexpr float sunSpeed = 0.0f;
    const auto& [cameraTransform, camera] = registry.get<TransformationComponent, CameraComponent>(game->camera);

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
    float factor = glm::clamp(2 * glm::sin(sun.angle), 0.0f, 1.0f);

    sun.ambient = (0.3f * factor + 0.1f) * sunLight.ambient;
    sun.diffuse = factor * sunLight.diffuse;
    sun.specular = factor * sunLight.specular;

    sun.direction = -glm::vec3(glm::cos(sun.angle), glm::sin(sun.angle), 0.0f);

    float sunDistance = camera.far - 25.0f * glm::cos(sun.angle - glm::radians(camera.pitch));
    sunTransform.position = -sunDistance * sun.direction + glm::vec3(cameraTransform.position.x, 0.0f, cameraTransform.position.z);
    sunTransform.rotation = glm::quat(glm::cos(sun.angle / 2), 0, 0, glm::sin(sun.angle / 2));
    sunTransform.calculateTransform();

    EntityMoveEvent e{game->sun};
    game->raiseEvent(e);
}

void EnvironmentSystem::destroyEntities() {
    while (entitiesToDestroy.size() > 0) {
        auto entity = entitiesToDestroy.front();

        if (registry.valid(entity)) {
            registry.destroy(entity);
        }

        entitiesToDestroy.pop();
    }
}

void EnvironmentSystem::clearCells() {
    while (cellsToClear.size() > 0) {
        const glm::ivec2& position = cellsToClear.front();

        registry.view<EnvironmentComponent, InstancedMeshComponent, TransformationComponent>()
            .each([&](const EnvironmentComponent& environment, InstancedMeshComponent& instancedMesh, const TransformationComponent& transformation) {
                auto it = instancedMesh.transformations.begin();
                bool needsUpdate = false;

                while (it != instancedMesh.transformations.end()) {
                    const glm::vec3& objectPosition = it->operator[](3);
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
}

void EnvironmentSystem::update(float dt) {
    // TODO: Optimize this
    clearCells();

    destroyEntities();

    SunLightComponent& sunLight = registry.get<SunLightComponent>(game->sun);
    TransformationComponent& sunTransform = registry.get<TransformationComponent>(game->sun);
    updateDayNightCycle(dt, sunTransform, sunLight);
}

void EnvironmentSystem::handleBuildEvent(const BuildEvent& e) {

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

void EnvironmentSystem::handleChunkCreatedEvent(const ChunkCreatedEvent& e) const {
    MeshPtr treeMesh = resourceManager.getResource<Mesh<>>("TREE_MESH");
    const TerrainComponent& terrain = registry.get<TerrainComponent>(e.entity);
    std::unordered_map<std::string, InstancedMesh<glm::mat4>> transformations;

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

            float cosAngle = glm::cos(angle);
            float sinAngle = glm::sin(angle);

            transformations[treeNames[type]].transformations.emplace_back(
                glm::vec4(scale.x * cosAngle, 0, -scale.x * sinAngle, 0),
                glm::vec4(0, scale.y, 0, 0),
                glm::vec4(scale.z * sinAngle, 0, scale.z * cosAngle, 0),
                glm::vec4(cosAngle * position.x + sinAngle * position.z, position.y, -sinAngle * position.x + cosAngle * position.z, 1));
        }
    }

    for (auto& [name, instancedMesh] : transformations) {
        instancedMesh.instanceBuffer.fillBuffer(instancedMesh.transformations);
    }

    MultiInstancedMeshComponent& instancedMesh = registry.emplace<MultiInstancedMeshComponent>(e.entity, treeMesh, transformations);
    registry.emplace<EnvironmentComponent>(e.entity);
}
