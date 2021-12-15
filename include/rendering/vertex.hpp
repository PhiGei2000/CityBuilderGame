#pragma once
#include <glm/glm.hpp>

namespace trafficSimulation {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
    };
} // namespace trafficSimulation
