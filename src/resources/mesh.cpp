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

#include "misc/roads/roadTile.hpp"

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

void Mesh::renderObject(ShaderPtr shader, const std::string& name) const {
    const auto& object = geometries.at(name);
    for (const auto& [material, geometry] : object) {
        renderGeometry(shader, material, geometry);
    }
}

template<>
void Mesh::linkInstanceBuffer<glm::mat4>(const InstanceBuffer& buffer) const {
    for (const auto& [_, subMesh] : geometries) {
        for (const auto& [_, geometry] : subMesh) {
            for (int i = 0; i < 4; i++) {
                geometry->setVertexAttribute(MeshGeometry::meshVertexAttributes.size() + i, VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4))}, buffer.vbo, 1);
            }
        }
    }
}

template<>
void Mesh::linkInstanceBuffer<RoadRenderData>(const InstanceBuffer& buffer) const {
    int stride = sizeof(glm::vec2) + sizeof(int);
    int offset = MeshGeometry::meshVertexAttributes.size();
    for (const auto& [_, subMesh] : geometries) {
        for (const auto& [_, geometry] : subMesh) {
            geometry->setVertexAttribute(offset + 1, VertexAttribute{2, GL_FLOAT, GL_FALSE, stride, (void*)0}, buffer.vbo, 1);
            geometry->setVertexAttribute(offset + 2, VertexAttribute{1, GL_INT, GL_FALSE, stride, (void*)(sizeof(glm::vec2))}, buffer.vbo, 1);
        }
    }
}
