#pragma once
#include <glm/glm.hpp>

struct Vertex {
    /// @brief The vertex position
    glm::vec3 position;
    /// @brief The vertex texture coordinate
    glm::vec2 texCoord;
    /// @brief The vertex normal
    glm::vec3 normal;
    /// @brief The tangent of the vertex
    glm::vec3 tangent;
    /// @brief The bitangent of the vertex
    glm::vec3 bitangent;

    /// @brief Creates a vertex with the specified parameters
    /// @param position The vertex position
    /// @param textureCoord The vertex texture coordinate
    /// @param normal The vertex normal
    Vertex(const glm::vec3& position, const glm::vec2& textureCoord, const glm::vec3& normal, const glm::vec3& tangent = glm::vec3(0.0f), const glm::vec3& bitangent = glm::vec3(0.0f))
        : position(position), texCoord(textureCoord), normal(normal), tangent(tangent), bitangent(bitangent) {
    }

    /// @brief Creates a vertex with the specified parameters and a texture coordinate of 0.0
    /// @param position The vertex position
    /// @param normal The vertex normal
    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent = glm::vec3(0.0f), const glm::vec3& bitangent = glm::vec3(0.0f))
        : position(position), texCoord(0.0f), normal(normal), tangent(tangent), bitangent(bitangent) {
    }
};
