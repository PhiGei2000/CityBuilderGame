#pragma once
#include "rendering/geometry.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"

#include "misc/typedefs.hpp"

#include <glm/glm.hpp>
#include <vector>

class InstanceBuffer;

struct Mesh {
    ShaderPtr shader;

    std::unordered_map<std::string, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    Mesh(ShaderPtr shader);

    void render(const glm::mat4& model, Shader* shader = nullptr) const;
    void renderInstanced(unsigned int instancesCount, const glm::mat4& model, Shader* shader = nullptr) const;

    void linkInstanceBuffer(const InstanceBuffer& buffer) const;
};

using MeshPtr = ResourcePtr<Mesh>;
