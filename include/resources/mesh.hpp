/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
