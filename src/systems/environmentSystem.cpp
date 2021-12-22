#include "systems/environmentSystem.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "resourceManager.hpp"

#include "components/components.hpp"

#include <glm/glm.hpp>

EnvironmentSystem::EnvironmentSystem(Game* game)
    : System(game) {
    init();
}

void EnvironmentSystem::init() {
    const ResourceManager& resourceManager = game->getResourceManager();
    Geometry* treeGeometry = resourceManager.getResource<Geometry>("TREE_GEOMETRY");
    Texture* treeTexture = resourceManager.getResource<Texture>("TREE_TEXTURE");
    Shader* meshShader = resourceManager.getResource<Shader>("MESH_SHADER");

    for (int i = 0; i < 100; i++) {
        entt::entity entity = registry.create();

        glm::vec3 position = glm::vec3((float)rand() / RAND_MAX * 200 - 100, 0.0f, (float)rand() / RAND_MAX * 200 - 100);
        float angle = (float)rand() / RAND_MAX * 0.5f * M_PI;
        glm::vec3 scale = glm::vec3((float)rand() / RAND_MAX * 0.5f + 0.5f);

        registry.emplace<MeshComponent>(entity, treeGeometry, meshShader, treeTexture);
        registry.emplace<TransformationComponent>(entity,
                                                  position,
                                                  glm::quat(cos(angle * 0.5f), 0, sin(angle * 0.5f), 0),
                                                  scale)
            .calculateTransform();
    }
}

void EnvironmentSystem::update(int dt) {
}
