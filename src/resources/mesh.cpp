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
#include "resources/mesh.hpp"

#include "misc/configuration.hpp"
#include "rendering/instanceBuffer.hpp"
#include "rendering/shadowBuffer.hpp"

Mesh::Mesh() {
}

void Mesh::renderGeometry(ShaderPtr& shader, const MaterialPtr& material, const GeometryPtr& geometry) const {
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

void Mesh::renderInstancedGeometry(ShaderPtr& shader, const MaterialPtr& material, const GeometryPtr& geometry, unsigned int instancesCount) const {
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

void Mesh::render(ShaderPtr shader) const {
    for (const auto& [name, data] : geometries) {
        for (const auto& [material, geometry] : data) {
            renderGeometry(shader, material, geometry);
        }
    }
}

void Mesh::renderInstanced(ShaderPtr shader, const InstanceBuffer& instanceBuffer) const {
    linkInstanceBuffer(instanceBuffer);

    for (const auto& [name, data] : geometries) {
        for (const auto& [material, geometry] : data) {
            renderInstancedGeometry(shader, material, geometry, instanceBuffer.instancesCount);
        }
    }
}

void Mesh::renderObject(ShaderPtr shader, const std::string& name) const {
    const auto& object = geometries.at(name);
    for (const auto& [material, geometry] : object) {
        renderGeometry(shader, material, geometry);
    }
}

void Mesh::renderObjectInstanced(ShaderPtr shader, const std::string& name, const InstanceBuffer& instanceBuffer) const {
    const auto& object = geometries.at(name);

    linkInstanceBuffer(instanceBuffer);
    for (const auto& [material, geometry] : object) {
        renderInstancedGeometry(shader, material, geometry, instanceBuffer.instancesCount);
    }
}

void Mesh::linkInstanceBuffer(const InstanceBuffer& buffer) const {
    for (const auto& [_, subMesh] : geometries) {
        for (const auto& [_, geometry] : subMesh) {
            for (int i = 0; i < 4; i++) {
                geometry->setVertexAttribute(MeshGeometry::meshVertexAttributes.size() + i, VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4))}, buffer.vbo, 1);
            }
        }
    }
}
