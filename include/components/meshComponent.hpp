#pragma once
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include <glm/glm.hpp>
#include <string>

struct MeshComponent {
    Geometry* geometry;
    Shader* shader;
    Texture* texture;
};
