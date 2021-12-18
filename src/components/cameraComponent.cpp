#include "components/cameraComponent.hpp"

#include "components/transformationComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

void CameraComponent::calculateMatrices(const TransformationComponent& transform) {
    projectionMatrix = glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);

    calculateVectors();
    glm::vec3 cameraTarget = transform.position + front;

    viewMatrix = glm::lookAt(transform.position, cameraTarget, up);
}

void CameraComponent::calculateVectors() {
    glm::vec3 direction = glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

    front = glm::normalize(direction);

    constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(front, up));

    this->up = glm::normalize(glm::cross(right, front));
}