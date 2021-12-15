#include "components/cameraComponent.hpp"

#include "components/transformationComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace trafficSimulation::components {
    void CameraComponent::calculateMatrices(const TransformationComponent& transform) {
        projectionMatrix = glm::ortho(0.0f, viewWidth, 0.0f, viewHeight, 0.1f, 100.0f);

        glm::vec3 position = glm::vec3(transform.position, 1.0f);
        glm::vec3 up = glm::vec3(glm::sin(transform.angle), glm::cos(transform.angle), 0.0f);

        viewMatrix = glm::lookAt(position, position + front, up);
    }
} // namespace trafficSimulation::components