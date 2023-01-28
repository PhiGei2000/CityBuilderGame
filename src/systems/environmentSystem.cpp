#include "systems/environmentSystem.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "resources/resourceManager.hpp"

#include "components/components.hpp"
#include "events/events.hpp"
#include "misc/configuration.hpp"
#include "misc/utility.hpp"

#include <glm/glm.hpp>

#if WIN32
#define _USE_MATH_DEFINES

#include <math.h>
#endif

EnvironmentSystem::EnvironmentSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<BuildEvent>()
        .connect<&EnvironmentSystem::handleBuildEvent>(*this);
}

void EnvironmentSystem::init() {
    MeshPtr treeMesh = resourceManager.getResource<Mesh>("TREE_MESH");
    const TerrainComponent& terrain = registry.get<TerrainComponent>(game->terrain);
    std::vector<glm::vec3> offsets;
    offsets.reserve(100);

    // spawn trees
    for (int i = 0; i < 100; i++) {
        glm::vec2 gridPos = Configuration::worldSize / static_cast<float>(RAND_MAX) * glm::vec2(rand(), rand());
        glm::vec3 position = glm::vec3(gridPos.x, terrain.getHeightValue(gridPos), gridPos.y);
        /*float angle = (float)rand() / static_cast<float>(RAND_MAX) * 0.5f * M_PI;
        glm::vec3 scale = glm::vec3((float)rand() / static_cast<float>(RAND_MAX) * 0.5 + 1.5f);*/
        offsets.push_back(position);
    }

    entt::entity entity = registry.create();
    InstancedMeshComponent& instancedMesh = registry.emplace<InstancedMeshComponent>(entity, treeMesh, offsets);
    registry.emplace<TransformationComponent>(entity, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)).calculateTransform();
    registry.emplace<EnvironmentComponent>(entity);
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
    auto view = registry.view<EnvironmentComponent, TransformationComponent>();
    for (auto entity : view) {
        const TransformationComponent& transform = view.get<TransformationComponent>(entity);

        glm::ivec2 gridPos = glm::ivec2(floor(transform.position.x / Configuration::gridSize), floor(transform.position.z / Configuration::gridSize));

        if (e.gridPosition == gridPos) {
            entitiesToDestroy.push(entity);
        }
    }
}
