#pragma once
#include "configuration.hpp"

#include <glm/glm.hpp>

// types of coordinates
// 1. world coords
//      world coords are the default coordinates
// 2. grid coords
//      world coords but without the y component
// 3. chunk coords
//      grid coords of the chunk origin divided by the chunk size
// 4. chunk grid coords
//      grid coords relative to the chunk position

namespace utility {     
    inline glm::vec2 worldToGridCoords(const glm::vec3& worldCoords) {
        return glm::vec2(worldCoords.x, worldCoords.z);
    }

    inline glm::vec3 gridToWorldCoords(const glm::vec2& gridCoords, float y = 0.0f) {
        return glm::vec3(gridCoords.x, y, gridCoords.y);
    }

    inline glm::ivec2 worldToChunkCoords(const glm::vec3& worldCoords) {
        constexpr float factor = 1 / Configuration::chunkSize;
        return glm::floor(factor * worldToGridCoords(worldCoords));
    }

    inline glm::vec3 chunkToWorldCoords(const glm::ivec2& chunkCoords, float y = 0.0f) {
        return gridToWorldCoords(Configuration::chunkSize * glm::vec2(chunkCoords), y);
    }

    inline glm::vec2 worldToChunkGridCoords(const glm::vec3& worldCoords) {
        return worldToGridCoords(worldCoords) - Configuration::chunkSize * glm::vec2(worldToChunkCoords(worldCoords));
    }

    inline glm::vec2 chunkGridToGridCoords(const glm::vec2& chunkGridCoords, const glm::ivec2& chunkCoords) {
        return Configuration::chunkSize * glm::vec2(chunkCoords) + chunkGridCoords;
    }

    inline glm::vec3 chunkGridToWorldCoords(const glm::vec2& chunkGridCoords, const glm::ivec2& chunkCoords, float y) {        
        return gridToWorldCoords(chunkGridToGridCoords(chunkGridCoords, chunkCoords), y);
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
}; // namespace utilty
