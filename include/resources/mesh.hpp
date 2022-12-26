#pragma once
#include "rendering/geometry.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"

#include "misc/typedefs.hpp"

#include <glm/glm.hpp>
#include <vector>

struct Mesh {
    ShaderPtr shader;

    std::unordered_map<std::string, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    Mesh(ShaderPtr shader);

    void render(const glm::mat4& model, Shader* shader = nullptr) const;
};

using MeshPtr = ResourcePtr<Mesh>;
