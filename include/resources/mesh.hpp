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
#include "rendering/meshRenderData.hpp"
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
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0u * sizeof(glm::vec4), vbo, 1},
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 1u * sizeof(glm::vec4), vbo, 1},
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 2u * sizeof(glm::vec4), vbo, 1},
        VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 3u * sizeof(glm::vec4), vbo, 1},
    };
}

template<>
inline constexpr VertexAttributes getInstanceBufferVertexAttributes<TransformationComponent>(unsigned int vbo) {
    return getInstanceBufferVertexAttributes<glm::mat4>(vbo);
}

template<typename TKey = std::string>
struct Mesh {
  protected:
    inline void renderGeometry(const MaterialPtr& material, const GeometryPtr& geometry, Shader* shader = nullptr) const {
        bool blend = false;
        if (material) {
            if (shader == nullptr) {
                material->use(this->shader->defaultShader);
            }
            else {
                material->use(shader->defaultShader);
            }

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

    inline void renderInstancedGeometry(const MaterialPtr& material, const GeometryPtr& geometry, unsigned int instancesCount, Shader* shader = nullptr) const {
        bool blend = false;
        if (material) {
            if (shader == nullptr) {
                material->use(this->shader->instanced);
            }
            else {
                material->use(shader->instanced);
            }

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
    ShaderPtr shader;
    std::unordered_map<TKey, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    inline Mesh() {

    }

    inline Mesh(ShaderPtr shader, const std::unordered_map<TKey, std::vector<std::pair<MaterialPtr, GeometryPtr>>>& geometries = {})
        : shader(shader), geometries(geometries) {
    }

    inline void render(const MeshRenderData& renderData, Shader* shader = nullptr) const {
        if (shader == nullptr) {
            renderData.uploadToShader(this->shader->defaultShader);
        }
        else {
            renderData.uploadToShader(shader->defaultShader);
        }

        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                renderGeometry(material, geometry, shader);
            }
        }
    }

    template<typename T>
    inline void renderInstanced(const MeshRenderData& renderData, const InstanceBuffer& instanceBuffer, Shader* shader = nullptr) const {
        if (shader == nullptr) {
            renderData.uploadToShader(this->shader->instanced);
        }
        else {
            renderData.uploadToShader(shader->instanced);
        }

        linkInstanceBuffer<T>(instanceBuffer);
        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                renderInstancedGeometry(material, geometry, instanceBuffer.getInstancesCount(), shader);
            }
        }
    }

    inline void renderObject(const TKey& key, const MeshRenderData& renderData, Shader* shader = nullptr) const {
        const auto& object = geometries.at(key);

        if (shader == nullptr) {
            renderData.uploadToShader(this->shader->defaultShader);
        }
        else {
            renderData.uploadToShader(shader->defaultShader);
        }

        for (const auto& [material, geometry] : object) {
            renderGeometry(material, geometry, shader);
        }
    }

    template<typename T>
    inline void renderObjectInstanced(const TKey& key, const MeshRenderData& renderData, const InstanceBuffer& instanceBuffer, Shader* shader = nullptr) const {
        const auto& object = geometries.at(key);

        if (shader == nullptr) {
            renderData.uploadToShader(this->shader->instanced);
        }
        else {
            renderData.uploadToShader(shader->instanced);
        }

        linkInstanceBuffer<T>(instanceBuffer);
        for (const auto& [material, geometry] : object) {
            renderInstancedGeometry(material, geometry, instanceBuffer.getInstancesCount(), shader);
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
