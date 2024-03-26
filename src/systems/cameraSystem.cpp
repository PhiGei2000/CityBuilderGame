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
#include "systems/cameraSystem.hpp"

#include "components/components.hpp"
#include "events/cameraUpdateEvent.hpp"

#include "misc/coordinateTransform.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

void CameraSystem::init() {
    cameraEntity = registry.create();

    const TransformationComponent& transform = registry.emplace<TransformationComponent>(cameraEntity, glm::vec3(0.0f, 10.0f, 0.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
    registry.emplace<CameraComponent>(cameraEntity).calculateMatrices(transform);
}

CameraSystem::CameraSystem(Game* game)
    : System(game) {

    init();

    eventDispatcher.sink<FramebufferSizeEvent>()
        .connect<&CameraSystem::onFramebufferSize>(*this);
}

void CameraSystem::update(float dt) {
    CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    TransformationComponent& transform = registry.get<TransformationComponent>(cameraEntity);

    glm::vec3 xzCameraFront = glm::vec3(camera.front.x, 0.0f, camera.front.z);
    glm::vec3 xzCameraRight = glm::vec3(camera.right.x, 0.0f, camera.right.z);

    const static float cameraSpeed = 10.0f;
    const static float cameraRotationSpeed = 10.0f;

    glm::vec3 cameraMoveDirection = glm::vec3(0.0f);
    glm::vec2 cameraRotationDirection = glm::vec2(0.0f);

    if (game->getKey(GLFW_KEY_W) == GLFW_PRESS) {
        cameraMoveDirection += xzCameraFront;
    }
    if (game->getKey(GLFW_KEY_S) == GLFW_PRESS) {
        cameraMoveDirection -= xzCameraFront;
    }
    if (game->getKey(GLFW_KEY_D) == GLFW_PRESS) {
        cameraMoveDirection += xzCameraRight;
    }
    if (game->getKey(GLFW_KEY_A) == GLFW_PRESS) {
        cameraMoveDirection -= xzCameraRight;
    }
    if (game->getKey(GLFW_KEY_E) == GLFW_PRESS) {
        cameraRotationDirection.x += 1;
    }
    if (game->getKey(GLFW_KEY_Q) == GLFW_PRESS) {
        cameraRotationDirection.x -= 1;
    }
    if (game->getKey(GLFW_KEY_R) == GLFW_PRESS) {
        cameraRotationDirection.y += 1;
    }
    if (game->getKey(GLFW_KEY_F) == GLFW_PRESS) {
        cameraRotationDirection.y -= 1;
    }

    bool cameraPositionUpdated = false;
    if (cameraMoveDirection.x != 0 || cameraMoveDirection.z != 0) {
        transform.position += dt * cameraSpeed * glm::normalize(cameraMoveDirection);

        const glm::vec2& cameraGridPos = utility::worldToNormalizedWorldGridCoords(transform.position);
        transform.position.y = glm::max(0.0f, game->terrain.getTerrainHeight(cameraGridPos)) + Configuration::cameraHeight;

        cameraPositionUpdated = true;
    }

    bool cameraRotationUpdated = false;
    if (cameraRotationDirection.x != 0 || cameraRotationDirection.y != 0) {
        camera.yaw += dt * cameraRotationDirection.x * cameraRotationSpeed;
        camera.pitch = glm::clamp(camera.pitch + cameraRotationDirection.y * dt * cameraRotationSpeed, -89.0f, 89.0f);
        cameraRotationUpdated = true;
    }

    if (cameraPositionUpdated || cameraRotationUpdated) {
        camera.calculateMatrices(transform);

        CameraUpdateEvent event{cameraEntity, false, cameraPositionUpdated, cameraRotationUpdated};
        game->raiseEvent<CameraUpdateEvent>(event);
    }
}

void CameraSystem::onFramebufferSize(const FramebufferSizeEvent& e) {
    CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

    camera.width = e.width;
    camera.height = e.height;

    camera.calculateMatrices(cameraTransform);

    CameraUpdateEvent event{cameraEntity, true, false, false};
    game->raiseEvent<CameraUpdateEvent>(event);
}
