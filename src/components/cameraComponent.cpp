#include "components/cameraComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace trafficSimulation::components {
    void CameraComponent::calculateMatrices() {
        projectionMatrix = glm::ortho(0.0f, viewWidth, 0.0f, viewHeight, 0.1f, 100.0f);

        viewMatrix = glm::lookAt(glm::vec3(position.x, position.y, 1.0f),
                                 glm::vec3(position.x, position.y, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    }
} // namespace trafficSimulation::components