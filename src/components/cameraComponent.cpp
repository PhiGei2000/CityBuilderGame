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
#include "components/cameraComponent.hpp"

#include "components/transformationComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

void CameraComponent::calculateMatrices(const TransformationComponent& transform) {
    projectionMatrix = glm::perspective(glm::radians(fov), width / height, near, far);

    calculateVectors();
    glm::vec3 cameraTarget = transform.position + front;

    viewMatrix = glm::lookAt(transform.position, cameraTarget, up);
}

void CameraComponent::calculateVectors() {
    front = glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

    constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(front, up));

    this->up = glm::normalize(glm::cross(right, front));
}
