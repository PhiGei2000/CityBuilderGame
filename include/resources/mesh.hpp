#pragma once
#include "rendering/geometry.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"

#include "misc/typedefs.hpp"

#include <glm/glm.hpp>
#include <vector>

class InstanceBuffer;

struct Mesh {    
    std::unordered_map<std::string, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    Mesh();

    void render(ShaderPtr shader) const;
    void renderInstanced(ShaderPtr shader, unsigned int instancesCount) const;

    void linkInstanceBuffer(const InstanceBuffer& buffer) const;
};

using MeshPtr = ResourcePtr<Mesh>;
