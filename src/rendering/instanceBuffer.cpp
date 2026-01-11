#include "rendering/instanceBuffer.hpp"

#include "components/transformationComponent.hpp"
#include "misc/roads/roadTile.hpp"

InstanceBuffer::InstanceBuffer() {
    glGenBuffers(1, &vbo);
}

unsigned int InstanceBuffer::getVBO() const {
    return vbo;
}

unsigned int InstanceBuffer::getInstancesCount() const {
    return instancesCount;
}

void InstanceBuffer::clearBuffer() {
    instancesCount = 0;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
