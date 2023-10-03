#pragma once
#include "rendering/geometry.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"

#include "misc/typedefs.hpp"

#include <glm/glm.hpp>
#include <vector>

class InstanceBuffer;

struct Mesh {
  protected:
    void renderGeometry(ShaderPtr& shader, const MaterialPtr& material, const GeometryPtr& geometry) const;
    void renderInstancedGeometry(ShaderPtr& shader, const MaterialPtr& material, const GeometryPtr& geometry, unsigned int instancesCount) const;

  public:
    std::unordered_map<std::string, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    Mesh();

    void render(ShaderPtr shader) const;
    void renderInstanced(ShaderPtr shader, const InstanceBuffer& instanceBuffer) const;

    void renderObject(ShaderPtr shader, const std::string& name) const;
    void renderObjectInstanced(ShaderPtr shader, const std::string& name, const InstanceBuffer& instanceBuffer) const;

    void linkInstanceBuffer(const InstanceBuffer& buffer) const;
};

using MeshPtr = ResourcePtr<Mesh>;
