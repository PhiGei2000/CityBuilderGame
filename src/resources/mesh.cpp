#include "resources/mesh.hpp"

#include "misc/configuration.hpp"
#include "rendering/instanceBuffer.hpp"
#include "rendering/shadowBuffer.hpp"

Mesh::Mesh(ShaderPtr shader)
    : shader(shader) {
}

void Mesh::render(const glm::mat4& model, Shader* alternateShader) const {
    if (alternateShader == nullptr) {
        // use default shader
        shader->use();
        shader->setMatrix4("model", model);
        shader->setInt("shadowMaps", ShadowBuffer::depthMapOffset);

        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                if (material) {
                    material->use(shader.get());
                }

                geometry->draw();
            }
        }
    }
    else {
        alternateShader->use();
        alternateShader->setMatrix4("model", model);

        for (const auto& [_, data] : geometries) {
            for (const auto& [_, geometry] : data) {
                geometry->draw();
            }
        }
    }
}

void Mesh::renderInstanced(unsigned int instancesCount, const glm::mat4& model, Shader* alternateShader) const {
    if (alternateShader == nullptr) {
        // use default shader
        shader->use();
        shader->setMatrix4("model", model);
        shader->setInt("shadowMaps", ShadowBuffer::depthMapOffset);

        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                if (material) {
                    material->use(shader.get());
                }

                std::static_pointer_cast<MeshGeometry>(geometry)->drawInstanced(instancesCount);
            }
        }
    }
    else {
        alternateShader->use();
        alternateShader->setMatrix4("model", model);

        for (const auto& [_, data] : geometries) {
            for (const auto& [_, geometry] : data) {
                std::static_pointer_cast<MeshGeometry>(geometry)->drawInstanced(instancesCount);
            }
        }
    }
}

void Mesh::linkInstanceBuffer(const InstanceBuffer& buffer) const {
    for (const auto& [_, subMesh] : geometries) {
        for (const auto& [_, geometry] : subMesh) {
            geometry->setVertexAttribute(MeshGeometry::meshVertexAttributes.size(), VertexAttribute{3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)0}, buffer.vbo, 1);
        }
    }
}
