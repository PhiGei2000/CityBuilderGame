#include "rendering/geometry.hpp"

#include <GL/glew.h>

namespace trafficSimulation {
    Geometry::Geometry() {
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void Geometry::fillBuffers(GeometryData data) {
        bind();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);

        drawCount = data.indices.size();
    }

    void Geometry::bind() const {
        glBindVertexArray(vao);
    }

    void Geometry::draw() const {
        bind();

        glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
    }
} // namespace trafficSimulation