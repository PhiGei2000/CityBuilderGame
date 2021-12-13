#include "components/transformationComponent.hpp"

namespace trafficSimulation::components {
    void TransformationComponent::calculateTransform() {
        transform = glm::mat3(1.0f);

        transform[0].x = glm::cos(angle) * scale.x;
        transform[0].y = glm::sin(angle) * scale.y;

        transform[1].x = -glm::sin(angle) * scale.x;
        transform[1].y = glm::cos(angle) * scale.y;

        transform[2] = glm::vec3(position.x, position.y, 1.0f);
    }
} // namespace trafficSimulation::components