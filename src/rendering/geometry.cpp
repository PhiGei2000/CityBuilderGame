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
#include "rendering/geometry.hpp"

Geometry::Geometry(const VertexAttributes& attributes, int drawMode)
    : drawMode(drawMode), drawCount(0) {
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    unsigned int offset = 0;

    for (int i = 0; i < attributes.size(); i++) {
        const VertexAttribute& attribute = attributes[i];

        glVertexAttribPointer(i, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);
        glEnableVertexAttribArray(i);

        offset += attribute.stride;
    }

    // unbind buffers
    // make sure to unbind the vertex array object before the element buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::setVertexAttribute(unsigned int index, const VertexAttribute& attribute, unsigned int vbo, unsigned int divisor) const {
    glBindVertexArray(vao);
    glEnableVertexAttribArray(index);

    glBindBuffer(GL_ARRAY_BUFFER, vbo == 0 ? this->vbo : vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);

    if (divisor != 0) {
        glVertexAttribDivisor(index, divisor);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::bufferData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int usage) {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

    drawCount = indices.size();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::draw() const {
    glBindVertexArray(vao);

    glDrawElements(drawMode, drawCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Geometry::bindBuffer() const {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

const VertexAttributes MeshGeometry::meshVertexAttributes = VertexAttributes{
    {3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),                    (void*)0},
    {2, GL_FLOAT, GL_FALSE, 14 * sizeof(float),  (void*)(3 * sizeof(float))},
    {3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),  (void*)(5 * sizeof(float))},
    {3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),  (void*)(8 * sizeof(float))},
    {3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float))}
};

MeshGeometry::MeshGeometry()
    : Geometry(meshVertexAttributes) {
}

MeshGeometry::MeshGeometry(const GeometryData& data, unsigned int usage)
    : Geometry(meshVertexAttributes) {
    bufferData(data, usage);
}

void MeshGeometry::bufferData(const GeometryData& data, unsigned int usage) {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), usage);

    drawCount = data.indices.size();
    culling = data.culling;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshGeometry::bufferSubData(const std::vector<Vertex>& vertices, unsigned int offset) {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(Vertex), vertices.data());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshGeometry::draw() const {
    if (culling) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    Geometry::draw();
}

void MeshGeometry::drawInstanced(unsigned int instancesCount) const {
    if (culling) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    glBindVertexArray(vao);

    glDrawElementsInstanced(drawMode, drawCount, GL_UNSIGNED_INT, 0, instancesCount);

    glBindVertexArray(0);
}
