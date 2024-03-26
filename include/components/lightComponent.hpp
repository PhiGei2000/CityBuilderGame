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

#include "misc/configuration.hpp"

#include <vector>

#include <glm/glm.hpp>

struct CameraComponent;

struct LightComponent : public AssignableComponent {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::mat4 lightView[Configuration::SHADOW_CASCADE_COUNT];
    glm::mat4 lightProjection[Configuration::SHADOW_CASCADE_COUNT];

    LightComponent(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    void assignToEntity(const entt::entity entity, entt::registry& registry) const override;

    void calculateLightMatrices(const CameraComponent& camera);

  private:
    static std::vector<glm::vec4> getFrustumInWorldSpace(const glm::mat4& projection, const glm::mat4& view);

    std::pair<glm::mat4, glm::mat4> calculateLightMatrices(const CameraComponent& camera, float nearPlane, float farPlane) const;
};
