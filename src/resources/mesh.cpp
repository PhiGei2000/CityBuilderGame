#include "resources/mesh.hpp"

#include "misc/configuration.hpp"
#include "rendering/instanceBuffer.hpp"
#include "rendering/shadowBuffer.hpp"

Mesh::Mesh() {
}

void Mesh::render(ShaderPtr shader) const {
    for (const auto& [name, data] : geometries) {
        for (const auto& [material, geometry] : data) {
            if (material) {
                material->use(shader.get());
            }

            geometry->draw();
        }
    }
}

void Mesh::renderInstanced(ShaderPtr shader, unsigned int instancesCount) const {

    for (const auto& [name, data] : geometries) {
        for (const auto& [material, geometry] : data) {
            if (material) {
                material->use(shader.get());
            }

            std::static_pointer_cast<MeshGeometry>(geometry)->drawInstanced(instancesCount);
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
