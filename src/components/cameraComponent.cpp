#include "components/cameraComponent.hpp"

#include "components/transformationComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace trafficSimulation::components {
    void CameraComponent::calculateMatrices(const TransformationComponent& transform) {
        projectionMatrix = glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);

        calculateVectors(transform);
        glm::vec3 cameraTarget = transform.position + cameraFront;
    }

    void CameraComponent::calculateVectors(const TransformationComponent& transform) {
        // x = pitch, y = yaw, z = roll
        glm::vec3 eulerAngles = glm::eulerAngles(transform.rotation);

        glm::vec3 direction = glm::vec3(
            glm::cos(glm::radians(eulerAngles.y)) * glm::cos(glm::radians(eulerAngles.x)),
            glm::sin(glm::radians(eulerAngles.x)),
            glm::sin(glm::radians(eulerAngles.y)) * glm::cos(glm::radians(eulerAngles.x)));

        cameraFront = glm::normalize(direction);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRight = glm::normalize(glm::cross(up, direction));

        cameraUp = glm::normalize(glm::cross(direction, cameraRight));
    }
} // namespace trafficSimulation::components