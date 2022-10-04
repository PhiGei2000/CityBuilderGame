#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct VelocityComponent : public AssignableComponent {
    glm::vec3 linearVelocity = glm::vec3(0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f);

    inline VelocityComponent() {
    }

    inline VelocityComponent(const glm::vec3& linearVelocity, const glm::vec3& angularVelocity)
        : linearVelocity(linearVelocity), angularVelocity(angularVelocity) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<VelocityComponent>(entity, linearVelocity, angularVelocity);
    }
};
