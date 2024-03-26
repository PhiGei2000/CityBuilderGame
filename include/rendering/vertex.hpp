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
#pragma once
#include <tuple>

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

    static inline std::tuple<glm::vec3, glm::vec3, glm::vec3> calculateTangentSpace(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec2& t0, const glm::vec2& t1, const glm::vec2& t2) {
        const glm::vec3& e1 = p1 - p0;
        const glm::vec3& e2 = p2 - p0;
        const glm::vec3& normal = -glm::normalize(glm::cross(e1, e2));

        const glm::vec2& dUV1 = t1 - t0;
        const glm::vec2& dUV2 = t2 - t0;

        const float f = 1 / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
        glm::vec3 tangent = glm::normalize(f * (dUV2.y * e1 - dUV1.y * e2));
        tangent = glm::normalize(tangent - glm::dot(tangent, normal) * normal);
        const glm::vec3& bitangent = glm::normalize(glm::cross(normal, tangent));

        return {tangent, bitangent, normal};
    }
};
