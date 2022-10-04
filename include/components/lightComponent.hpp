#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct LightComponent : public AssignableComponent {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    inline LightComponent(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
        : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {
    }        

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<LightComponent>(entity, direction, ambient, diffuse, specular);
    }
};