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

enum class MeshShadingMode {
    WIREFRAME,
    SOLID
};

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
    std::map<TKey, std::vector<std::pair<MaterialPtr, GeometryPtr>>> geometries;

    inline Mesh() {
    }

    inline Mesh(ShaderPtr shader, const std::map<TKey, std::vector<std::pair<MaterialPtr, GeometryPtr>>>& geometries = {})
        : shader(shader), geometries(geometries) {
    }

    inline void render(const MeshRenderData& renderData, MeshShadingMode shadingMode, Shader* shader = nullptr) const {
        if (shader == nullptr) {
            renderData.uploadToShader(this->shader->defaultShader);
        }
        else {
            renderData.uploadToShader(shader->defaultShader);
        }

        int polygonMode = GL_FILL;
        switch (shadingMode) {
            case MeshShadingMode::WIREFRAME:
                polygonMode = GL_LINE;
                glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
                break;
            default:
                break;
        }

        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                renderGeometry(material, geometry, shader);
            }
        }

        if (polygonMode != GL_FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    template<typename T>
    inline void renderInstanced(const MeshRenderData& renderData, MeshShadingMode shadingMode, const InstanceBuffer& instanceBuffer, Shader* shader = nullptr) const {
        if (shader == nullptr) {
            renderData.uploadToShader(this->shader->instanced);
        }
        else {
            renderData.uploadToShader(shader->instanced);
        }

        int polygonMode = GL_FILL;
        switch (shadingMode) {
            case MeshShadingMode::WIREFRAME:
                polygonMode = GL_LINE;
                glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
                break;
            default:
                break;
        }

        linkInstanceBuffer<T>(instanceBuffer);
        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                renderInstancedGeometry(material, geometry, instanceBuffer.getInstancesCount(), shader);
            }
        }

        if (polygonMode != GL_FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
