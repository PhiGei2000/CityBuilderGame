#pragma once

#include <glm/glm.hpp>

namespace trafficSimulation::components {
    struct TransformationComponent;

    struct CameraComponent {  
        static constexpr glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

        float viewWidth;
        float viewHeight;

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        void calculateMatrices(const TransformationComponent& transform);
    };
} // namespace trafficSimulation::components