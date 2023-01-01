#pragma once
#include "component.hpp"

#include "misc/configuration.hpp"

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
};