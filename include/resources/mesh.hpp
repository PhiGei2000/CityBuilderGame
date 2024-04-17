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
#include "rendering/instanceBuffer.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"
#include "resources/roadGeometryGenerator.hpp"

#include "misc/typedefs.hpp"

#include <glm/glm.hpp>
#include <vector>

template<typename T>
constexpr VertexAttributes getInstanceBufferVertexAttributes(unsigned int vbo = 0);

template<>
inline constexpr VertexAttributes getInstanceBufferVertexAttributes<glm::mat4>(unsigned int vbo) {
    return VertexAttributes{
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0 * sizeof(glm::vec4)), vbo, 1},
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)), vbo, 1},
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)), vbo, 1},
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)), vbo, 1},
    };
}

template<>
inline constexpr VertexAttributes getInstanceBufferVertexAttributes<TransformationComponent>(unsigned int vbo) {
    return getInstanceBufferVertexAttributes<glm::mat4>(vbo);
}

template<typename TKey = std::string>
struct Mesh {
  protected:
    inline void renderGeometry(ShaderPtr& shader, const MaterialPtr& material, const GeometryPtr& geometry) const {
        bool blend = false;
        if (material) {
            material->use(shader.get());

            blend = material->dissolve < 1.0f;
        }

        if (blend) {
            glEnable(GL_BLEND);
        }

        geometry->draw();

        if (blend) {
            glDisable(GL_BLEND);
        }
    }

    inline void renderInstancedGeometry(ShaderPtr& shader, const MaterialPtr& material, const GeometryPtr& geometry, unsigned int instancesCount) const {
        bool blend = false;
        if (material) {
            material->use(shader.get());

            blend = material->dissolve < 1.0f;
        }

        if (blend) {
            glEnable(GL_BLEND);
        }

        std::static_pointer_cast<MeshGeometry>(geometry)->drawInstanced(instancesCount);

        if (blend) {
            glDisable(GL_BLEND);
        }
    }

  public:
    std::unordered_map<TKey, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    inline void render(ShaderPtr shader) const {
        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                renderGeometry(shader, material, geometry);
            }
        }
    }

    template<typename T>
    inline void renderInstanced(ShaderPtr shader, const InstanceBuffer& instanceBuffer) const {
        linkInstanceBuffer<T>(instanceBuffer);

        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                renderInstancedGeometry(shader, material, geometry, instanceBuffer.getInstancesCount());
            }
        }
    }

    inline void renderObject(ShaderPtr shader, const TKey& key) const {
        const auto& object = geometries.at(key);
        for (const auto& [material, geometry] : object) {
            renderGeometry(shader, material, geometry);
        }
    }

    template<typename T>
    inline void renderObjectInstanced(ShaderPtr shader, const TKey& key, const InstanceBuffer& instanceBuffer) const {
        const auto& object = geometries.at(key);

        linkInstanceBuffer<T>(instanceBuffer);
        for (const auto& [material, geometry] : object) {
            renderInstancedGeometry(shader, material, geometry, instanceBuffer.getInstancesCount());
        }
    }

    template<typename T>
    inline void linkInstanceBuffer(const InstanceBuffer& buffer) const {
        unsigned int vbo = buffer.getVBO();
        unsigned int offset = MeshGeometry::meshVertexAttributes.size();

        const VertexAttributes& vertexAttributes = getInstanceBufferVertexAttributes<T>(vbo);

        for (const auto& [_, subMesh] : geometries) {
            for (const auto& [_, geometry] : subMesh) {
                for (int i = 0; i < vertexAttributes.size(); i++) {
                    geometry->setVertexAttribute(offset + i, vertexAttributes[i]);
                }
            }
        }
    }
};

using MeshPtr = ResourcePtr<Mesh<>>;
