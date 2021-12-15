#pragma once
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"

#include <glm/glm.hpp>
#include <string>

namespace trafficSimulation::components {
    struct MeshComponent {
        Geometry* geometry;
        glm::vec4 color;
        Shader* shader;
    };
}; // namespace trafficSimulation::components
