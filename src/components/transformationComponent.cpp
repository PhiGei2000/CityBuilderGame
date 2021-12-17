#include "components/transformationComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace trafficSimulation::components {
    void TransformationComponent::calculateTransform() {
        transform = glm::mat4(1.0f);

        transform = glm::scale(transform, this->scale);

        transform = glm::toMat4(rotation) * transform;
        transform = glm::translate(transform, position);
    }

    void TransformationComponent::translate(const glm::vec3& translation) {
        position += translation;
    }

    void TransformationComponent::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void TransformationComponent::rotate(const glm::vec3& axis, float angle) {
        glm::quat dRot = glm::angleAxis(angle, axis);

        rotation = dRot * rotation;
    }

    void TransformationComponent::setRotation(const glm::vec3& axis, float angle) {
        rotation = glm::angleAxis(angle, axis);
    }

    void TransformationComponent::setRotation(const glm::vec3& eulerAngles) {
        rotation = glm::quat(eulerAngles);
    }

    void TransformationComponent::addScale(const glm::vec3& scale) {
        this->scale.x *= scale.x;
        this->scale.y *= scale.y;
        this->scale.z *= scale.z;
    }

    void TransformationComponent::setScale(const glm::vec3& scale) {
        this->scale = scale;
    }
} // namespace trafficSimulation::components