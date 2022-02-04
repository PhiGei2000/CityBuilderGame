#pragma once

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

struct Mesh {
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
};
