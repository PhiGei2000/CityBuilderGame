#include "systems/buildSystem.hpp"

#include "components/components.hpp"
#include "events/events.hpp"

#include "misc/utility.hpp"

#include "GLFW/glfw3.h"

#include <iostream>

BuildSystem::BuildSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<MouseButtonEvent>()
        .connect<&BuildSystem::handleMouseButtonEvent>(*this);

    eventDispatcher.sink<KeyEvent>()
        .connect<&BuildSystem::handleKeyEvent>(*this);

    eventDispatcher.sink<BuildEvent>()
        .connect<&BuildSystem::handleBuildEvent>(*this);
}

void BuildSystem::init() {
    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();

    buildMarkerEntity = registry.create();
    registry.emplace<BuildMarkerComponent>(buildMarkerEntity, glm::ivec2(0.0f), false);
    registry.emplace<TransformationComponent>(buildMarkerEntity, glm::vec3(0.0f), glm::quat(), glm::vec3(1.0f));
    registry.emplace<MeshComponent>(buildMarkerEntity,
                                    resourceManager.getResource<Geometry>("BUILDMARKER_GEOMETRY"),
                                    resourceManager.getResource<Shader>("MESH_SHADER"),
                                    resourceManager.getResource<Material>("BUILDMARKER_MATERIAL"));
}

void BuildSystem::update(float dt) {
    BuildMarkerComponent& buildMarkerComponent = registry.get<BuildMarkerComponent>(buildMarkerEntity);
    TransformationComponent& transform = registry.get<TransformationComponent>(buildMarkerEntity);

    if (game->getState() == GameState::BUILD_MODE) {
        // get grid position
        const glm::vec2& mousePos = game->getMousePos();
        glm::ivec2 gridPos = getGridPos(mousePos);

        if (this->state.building) {
        }
        else {
            // display build marker and update the build marker position
            buildMarkerComponent.visible = true;

            if (gridPos != buildMarkerComponent.pos) {
                buildMarkerComponent.pos = gridPos;
                transform.position = glm::vec3(gridPos.x * Configuration::gridSize, 0.1f, gridPos.y * Configuration::gridSize);
                transform.calculateTransform();
            }
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

    float gridX = intersectionPoint.x / Configuration::gridSize;
    float gridZ = intersectionPoint.z / Configuration::gridSize;

    return glm::ivec2(glm::floor(gridX), glm::floor(gridZ));
}

void BuildSystem::setState(BuildingType selectedType, bool building, const glm::ivec2& startPosition) {
    state.building = building;
    state.startPosition = building ? startPosition : glm::ivec2(-1);
    state.selectedBuildingType = selectedType;
}

void BuildSystem::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (e.action == GLFW_RELEASE) {
        if (game->getState() == GameState::BUILD_MODE) {
            const BuildMarkerComponent& buildMarker = registry.get<BuildMarkerComponent>(buildMarkerEntity);

            int x = buildMarker.pos.x, y = buildMarker.pos.y;
            int grid = Configuration::worldSize / Configuration::gridSize;

            if (utility::inRange(x, 0, grid) && utility::inRange(y, 0, grid)) {
                BuildingType selectedType = state.selectedBuildingType;
                BuildAction action = BuildAction::DEFAULT;

                BuildEvent event;
                switch (selectedType) {
                    case BuildingType::ROAD:
                        action = state.building ? BuildAction::END : BuildAction::BEGIN;
                        setState(selectedType, !state.building, buildMarker.pos);
                        break;
                    default:
                        break;
                }

                event = BuildEvent{buildMarker.pos, selectedType, action};
                game->raiseEvent(event);
            }
        }
    }
}

void BuildSystem::handleKeyEvent(const KeyEvent& e) {
    if (e.action == GLFW_PRESS) {
        GameState state = game->getState();

        if (e.key == GLFW_KEY_B) {
            if (state == GameState::RUNNING) {
                game->setState(GameState::BUILD_MODE);
            }
        }
        else if (e.key == GLFW_KEY_ESCAPE) {
            if (state == GameState::BUILD_MODE) {
                game->setState(GameState::RUNNING);
            }
        }
    }
}

void BuildSystem::handleBuildEvent(const BuildEvent& e) {
    if (e.action != BuildAction::SELECT) {
        return;
    }

    setState(e.type, false);
}
