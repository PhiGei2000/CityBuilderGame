#include "components/transformationComponent.hpp"

#include <glm/gtx/transform.hpp>

TransformationComponent::TransformationComponent(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
    : position(position), rotation(rotation), scale(scale) {
    calculateTransform();
}

void TransformationComponent::calculateTransform() {
    glm::mat4 rotation = glm::toMat4(this->rotation);

    transform = glm::translate(position) * glm::scale(scale) * rotation;
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