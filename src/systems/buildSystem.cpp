#include "systems/buildSystem.hpp"

#include "components/components.hpp"
#include "events/events.hpp"

#include <iostream>

BuildSystem::BuildSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<MouseButtonEvent>()
        .connect<&BuildSystem::handleMouseButtonEvent>(*this);
}

void BuildSystem::init() {
    int dimension = 2 * worldSize / gridSize;
    usedSpace = new bool*[dimension];
    for (int i = 0; i < dimension; i++) {
        usedSpace[i] = new bool[dimension];
        for (int j = 0; j < dimension; j++) {
            usedSpace[i][j] = false;
        }
    }

    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();
    const ResourceManager& resourceManager = game->getResourceManager();

    buildMarkerEntity = registry.create();
    registry.emplace<BuildMarkerComponent>(buildMarkerEntity, glm::ivec2(0.0f), false);
    registry.emplace<TransformationComponent>(buildMarkerEntity, glm::vec3(0.0f), glm::quat(), glm::vec3(1.0f));
    registry.emplace<MeshComponent>(buildMarkerEntity,
                                    resourceManager.getResource<Geometry>("BUILDMARKER_GEOMETRY"),
                                    resourceManager.getResource<Shader>("MESH_SHADER"),
                                    resourceManager.getResource<Texture>("BUILDMARKER_TEXTURE"));
}

void BuildSystem::update(int dt) {
    BuildMarkerComponent& buildMarkerComponent = registry.get<BuildMarkerComponent>(buildMarkerEntity);
    TransformationComponent& transform = registry.get<TransformationComponent>(buildMarkerEntity);

    if (game->getState() == GameState::BUILD_MODE) {
        const glm::vec2& mousePos = game->getMousePos();
        glm::ivec2 gridPos = getGridPos(mousePos);

        buildMarkerComponent.visible = true;

        if (gridPos != buildMarkerComponent.pos) {
            buildMarkerComponent.pos = gridPos;
            transform.position = glm::vec3(gridPos.x * gridSize, 0.1f, gridPos.y * gridSize);
            transform.calculateTransform();
        }
    }
    else {
        buildMarkerComponent.visible = false;
    }
}

glm::ivec2 BuildSystem::getGridPos(const glm::vec2& mousePos) const {
    const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraPos = registry.get<TransformationComponent>(cameraEntity);

    glm::vec4 ray_clip = glm::vec4(mousePos, -1.0f, 1.0f);
    glm::vec4 ray_eye = glm::inverse(camera.projectionMatrix) * ray_clip;
    ray_eye.z = -1;
    ray_eye.w = 0;

    glm::vec3 direction = glm::normalize(glm::vec3(glm::inverse(camera.viewMatrix) * ray_eye));

    float lambda = -cameraPos.position.y / direction.y;

    glm::vec3 intersectionPoint = cameraPos.position + lambda * direction;

    float gridX = intersectionPoint.x / gridSize;
    float gridZ = intersectionPoint.z / gridSize;

    return glm::ivec2(glm::floor(gridX), glm::floor(gridZ));
}

void BuildSystem::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (game->getState() == GameState::BUILD_MODE) {
        const BuildMarkerComponent& buildMarker = registry.get<BuildMarkerComponent>(buildMarkerEntity);

        int x = buildMarker.pos.x, y = buildMarker.pos.y;
        int grid = worldSize / gridSize;

        if (x > -grid && x <= grid && y >= -grid && y <= grid) {
            if (usedSpace[x + grid][y + grid]) {
                return;
            }

            usedSpace[x + grid][y + grid] = true;
            const ResourceManager& resourceManager = game->getResourceManager();
            entt::entity street = registry.create();

            registry.emplace<TransformationComponent>(street, glm::vec3(buildMarker.pos.x * gridSize, 0.0f, buildMarker.pos.y * gridSize), glm::quat(), glm::vec3(1.0f));
            registry.emplace<MeshComponent>(street,
                                            resourceManager.getResource<Geometry>("STREET_STRAIGHT_GEOMETRY"),
                                            resourceManager.getResource<Shader>("MESH_SHADER"),
                                            resourceManager.getResource<Texture>("STREET_TEXTURE"));
        }
    }
}
