#pragma once

#include <glm/glm.hpp>

namespace trafficSimulation::components {
    struct TransformationComponent {
        glm::vec2 position;
        float angle;
        glm::vec2 scale;

        glm::mat4 transform;

        void calculateTransform();
    };
} // namespace trafficSimulation::components