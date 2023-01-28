#include "rendering/instanceBuffer.hpp"

#include <GL/glew.h>

InstanceBuffer::InstanceBuffer() {
    glGenBuffers(1, &vbo);   
}

void InstanceBuffer::fillBuffer(const std::vector<glm::vec3>& offsets) const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(glm::vec3), &offsets[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
