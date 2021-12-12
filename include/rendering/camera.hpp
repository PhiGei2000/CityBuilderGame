#pragma once

#include <glm/glm.hpp>

namespace trafficSimulation {
    struct Camera {
        glm::vec2 position = glm::vec2(0.0, 0.0);

        float viewWidth = 64.0f;
        float viewHeight = 64.0f;

        glm::mat4 getProjectionMatrix() const;

        glm::mat4 getViewMatrix() const;
    };
} // namespace trafficSimulation