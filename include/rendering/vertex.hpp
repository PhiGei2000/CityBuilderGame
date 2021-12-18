#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;

    Vertex(const glm::vec3& position, const glm::vec2& textureCoord, const glm::vec3& normal)
        : position(position), texCoord(textureCoord), normal(normal) {
    }
};
