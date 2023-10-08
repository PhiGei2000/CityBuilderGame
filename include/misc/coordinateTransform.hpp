#pragma once
#include "configuration.hpp"

#include <glm/glm.hpp>

// types of coordinates
// 1. world coords
//      world coords are the default coordinates
// 2. normalized world grid coords
//      grid coords divided by cell size
// 3. normalized chunk grid coords
//      chunk grid coords divided by cell size

namespace utility {
    inline glm::vec2 worldToNormalizedWorldGridCoords(const glm::vec3& position) {
        return 1.0f / Configuration::cellSize * glm::vec2(position.x, position.z);
    }

    inline std::tuple<glm::ivec2, glm::vec2> normalizedWorldGridToNormalizedChunkGridCoords(const glm::vec2& position) {
        const glm::ivec2& chunk = glm::floor(1.0f / Configuration::cellsPerChunk * position);

        return {chunk, position - glm::vec2(Configuration::cellsPerChunk * chunk)};
    }

    inline std::tuple<glm::ivec2, glm::ivec2> normalizedWorldGridToNormalizedChunkGridCoords(const glm::ivec2& position) {
        const glm::ivec2& chunk = glm::floor(1.0f / Configuration::cellsPerChunk * glm::vec2(position));

        return {chunk, position - Configuration::cellsPerChunk * chunk};
    }

    inline std::tuple<glm::ivec2, glm::vec2> worldToNormalizedChunkGridCoords(const glm::vec3& position) {
        return normalizedWorldGridToNormalizedChunkGridCoords(worldToNormalizedWorldGridCoords(position));
    }

    inline glm::vec3 normalizedWorldGridToWorldCoords(const glm::vec2& position, float y = 0) {
        return static_cast<float>(Configuration::cellSize) * glm::vec3(position.x, y, position.y);
    }

    inline glm::vec2 normalizedChunkGridToNormalizedWorldGridCoords(const glm::ivec2& chunk, const glm::vec2& position) {
        return glm::vec2(Configuration::cellsPerChunk * chunk) + position;
    }

    inline glm::ivec2 normalizedChunkGridToNormalizedWorldGridCoords(const glm::ivec2& chunk, const glm::ivec2& position) {
        return Configuration::cellsPerChunk * chunk + position;
    }

    inline glm::vec3 normalizedChunkGridToWorldCoords(const glm::ivec2& chunk, const glm::vec2& position, float y = 0) {
        return normalizedWorldGridToWorldCoords(normalizedChunkGridToNormalizedWorldGridCoords(chunk, position), y);
    }

    inline glm::vec3 cartesianToSpherical(const glm::vec3& coords) {
        float r = glm::length(coords);

        float theta = glm::acos(coords.y / r);
        float phi = (coords.x == 0 && coords.z == 0) ? 0 : glm::sign(coords.x) * glm::acos(coords.x / glm::sqrt(coords.x * coords.x + coords.z * coords.z));
        return glm::vec3(r, theta, phi);
    }

    inline glm::vec3 cartesianToSpherical(float x, float y, float z) {
        return cartesianToSpherical(glm::vec3(x, y, z));
    }

    inline glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
        return r * glm::vec3(
                       glm::sin(theta) * glm::cos(phi),
                       glm::cos(theta),
                       glm::sin(theta) * glm::sin(phi));
    }

    inline glm::vec3 sphericalToCartesian(const glm::vec3& coords) {
        return sphericalToCartesian(coords.x, coords.y, coords.z);
    }
}; // namespace utility
