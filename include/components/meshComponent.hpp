#pragma once
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include <glm/glm.hpp>
#include <string>

struct MeshComponent {
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
};
