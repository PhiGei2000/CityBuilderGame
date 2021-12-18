#pragma once

#include <glm/glm.hpp>

namespace trafficSimulation::components {
    struct TransformationComponent;

    struct CameraComponent {
        glm::vec3 cameraUp, cameraFront, cameraRight;

        float width;
        float height;
        float fov = 60.0f;

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        void calculateMatrices(const TransformationComponent& transform);

        void calculateVectors(const TransformationComponent& transform);
    };
} // namespace trafficSimulation::components