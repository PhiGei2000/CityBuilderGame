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
#pragma once
#include "component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformationComponent : public AssignableComponent {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    glm::mat4 transform;

    TransformationComponent(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

    void calculateTransform();

    void translate(const glm::vec3& translation);
    void setPosition(const glm::vec3& position);

    void rotate(const glm::vec3& axis, float angle);
    void setRotation(const glm::vec3& axis, float angle);
    void setRotation(const glm::vec3& eulerAngles);

    void addScale(const glm::vec3& scale);
    void setScale(const glm::vec3& scale);    

    void assignToEntity(const entt::entity entity, entt::registry& registry) const override;
};
