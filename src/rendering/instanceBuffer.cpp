#include "rendering/instanceBuffer.hpp"

#include "components/transformationComponent.hpp"

#include <GL/glew.h>

InstanceBuffer::InstanceBuffer() {
    glGenBuffers(1, &vbo);   
}

void InstanceBuffer::fillBuffer(const std::vector<TransformationComponent>& transformations) const {
    std::vector<glm::mat4> matrices;
    matrices.reserve(transformations.size());

    for (const auto& transform : transformations) {
        matrices.push_back(transform.transform);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
