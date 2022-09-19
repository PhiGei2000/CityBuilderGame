#pragma once
#include <glm/glm.hpp>

struct Vertex {
    /// @brief The vertex position
    glm::vec3 position;
    /// @brief The vertex texture coordinate
    glm::vec2 texCoord;
    /// @brief The vertex normal
    glm::vec3 normal;

    /// @brief Creates a vertex with the specified parameters
    /// @param position The vertex position
    /// @param textureCoord The vertex texture coordinate
    /// @param normal The vertex normal
    Vertex(const glm::vec3& position, const glm::vec2& textureCoord, const glm::vec3& normal)
        : position(position), texCoord(textureCoord), normal(normal) {
    }

    /// @brief Creates a vertex with the specified parameters and a texture coordinate of 0.0
    /// @param position The vertex position
    /// @param normal The vertex normal
    Vertex(const glm::vec3& position, const glm::vec3& normal)
        : position(position), texCoord(0.0f), normal(normal) {
    }
};
