#pragma once
#include "rendering/material.hpp"
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"

#include "misc/typedefs.hpp"

#include <vector>
#include <glm/glm.hpp>

struct Mesh {
    ShaderPtr shader;

    std::unordered_map<std::string, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    Mesh(ShaderPtr shader);

    void render(const glm::mat4& model) const;
};

using MeshPtr = ResourcePtr<Mesh>;
