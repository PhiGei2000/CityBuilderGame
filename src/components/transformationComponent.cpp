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
    this->scale *= scale;
}

void TransformationComponent::setScale(const glm::vec3& scale) {
    this->scale = scale;
}

void TransformationComponent::assignToEntity(const entt::entity entity, entt::registry& registry) const {
    registry.emplace<TransformationComponent>(entity, position, rotation, scale).calculateTransform();
}
