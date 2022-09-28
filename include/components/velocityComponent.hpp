#pragma once

#include <glm/glm.hpp>

struct VelocityComponent {
    glm::vec3 linearVelocity = glm::vec3(0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f);

    inline VelocityComponent() {
    }

    inline VelocityComponent(const glm::vec3& linearVelocity, const glm::vec3& angularVelocity)
        : linearVelocity(linearVelocity), angularVelocity(angularVelocity) {
    }
};
