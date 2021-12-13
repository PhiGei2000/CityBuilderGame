#pragma once

#include <glm/glm.hpp>

namespace trafficSimulation::components {
    struct CameraComponent {
        glm::vec2 position = glm::vec2(0.0f, 0.0f);

        float viewWidth;
        float viewHeight;

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        void calculateMatrices();
    };
} // namespace trafficSimulation::components