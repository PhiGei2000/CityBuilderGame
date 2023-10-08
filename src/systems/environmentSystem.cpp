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
}

void EnvironmentSystem::updateDayNightCycle(float dt, TransformationComponent& sunTransform, SunLightComponent& sunLight) const {
    // sun movement
    const float sunSpeed = 0.05f;

    // move sun
    static constexpr float two_pi = 2 * glm::pi<float>();
    sunLight.angle += sunSpeed * dt;
    if (sunLight.angle > two_pi) {
        sunLight.angle -= two_pi;
    }
    else if (sunLight.angle < -two_pi) {
        sunLight.angle += two_pi;
    }

    sunLight.direction = -glm::vec3(glm::cos(sunLight.angle), glm::sin(sunLight.angle), 0.0f);
    sunTransform.position = -300.0f * sunLight.direction;

#if DEBUG
    std::cout << "sun pos: " << sunTransform.position << "(phi: " << sunLight.angle << ")" << std::endl;
#endif

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
    return;

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
    MeshPtr treeMesh = resourceManager.getResource<Mesh>("TREE_MESH");
    const TerrainComponent& terrain = registry.get<TerrainComponent>(e.entity);
    std::unordered_map<std::string, InstancedMesh> transformations;

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
