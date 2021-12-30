#pragma once

#include <glm/glm.hpp>

struct MovementComponent {
    glm::vec3 linearVelocity;
    glm::vec3 angularVelocity;
};
