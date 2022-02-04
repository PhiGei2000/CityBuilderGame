#include "systems/environmentSystem.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "resourceManager.hpp"

#include "components/components.hpp"
#include "events/events.hpp"
#include "misc/configuration.hpp"

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
    const ResourceManager& resourceManager = game->getResourceManager();
    std::shared_ptr<Geometry> treeGeometry = resourceManager.getResource<Geometry>("TREE_GEOMETRY");
    std::shared_ptr<Texture> treeTexture = resourceManager.getResource<Texture>("TREE_TEXTURE");
    std::shared_ptr<Shader> meshShader = resourceManager.getResource<Shader>("MESH_SHADER");

    // spawn trees
    for (int i = 0; i < 100; i++) {
        entt::entity entity = registry.create();

        glm::vec3 position = glm::vec3((float)rand() / RAND_MAX * Configuration::worldSize, 0.0f, (float)rand() / RAND_MAX * Configuration::worldSize);
        float angle = (float)rand() / RAND_MAX * 0.5f * M_PI;
        glm::vec3 scale = glm::vec3((float)rand() / RAND_MAX * 0.5 + 1.5f);

        registry.emplace<MeshComponent>(entity, treeGeometry, meshShader, treeTexture);
        registry.emplace<TransformationComponent>(entity,
                                                  position,
                                                  glm::quat(cos(angle * 0.5f), 0, sin(angle * 0.5f), 0),
                                                  scale)
            .calculateTransform();
        registry.emplace<EnvironmentComponent>(entity);
    }
}

void EnvironmentSystem::update(float dt) {
    while (entitiesToDestroy.size() > 0) {
        auto entity = entitiesToDestroy.front();

        registry.destroy(entity);

        entitiesToDestroy.pop();
    }
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