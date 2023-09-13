#include "rendering/instanceBuffer.hpp"

#include "components/transformationComponent.hpp"

#include <GL/glew.h>

InstanceBuffer::InstanceBuffer() {
    glGenBuffers(1, &vbo);
}

void InstanceBuffer::fillBuffer(const std::vector<TransformationComponent>& transformations) {
    std::vector<glm::mat4> matrices;
    matrices.reserve(transformations.size());

    for (const auto& transform : transformations) {
        matrices.push_back(transform.transform);
    }
    instancesCount = matrices.size();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, instancesCount * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
