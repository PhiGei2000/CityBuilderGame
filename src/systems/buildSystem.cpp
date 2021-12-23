#include "systems/buildSystem.hpp"

#include "components/components.hpp"

#include <iostream>

BuildSystem::BuildSystem(Game* game)
    : System(game) {
    init();
}

void BuildSystem::init() {
    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();
    const ResourceManager& resourceManager = game->getResourceManager();

    buildMarker = registry.create();
    registry.emplace<BuildMarkerComponent>(buildMarker, glm::ivec2(0.0f), false);
    registry.emplace<TransformationComponent>(buildMarker, glm::vec3(0.0f), glm::quat(), glm::vec3(1.0f));
    registry.emplace<MeshComponent>(buildMarker,
                                    resourceManager.getResource<Geometry>("BUILDMARKER_GEOMETRY"),
                                    resourceManager.getResource<Shader>("MESH_SHADER"),
                                    resourceManager.getResource<Texture>("BUILDMARKER_TEXTURE"));
}

void BuildSystem::update(int dt) {
    BuildMarkerComponent& buildMarkerComponent = registry.get<BuildMarkerComponent>(buildMarker);
    TransformationComponent& transform = registry.get<TransformationComponent>(buildMarker);

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

    float gridX = intersectionPoint.x / gridSize + 1;
    float gridZ = intersectionPoint.z / gridSize;

    return glm::ivec2(glm::floor(gridX), glm::floor(gridZ));
}
