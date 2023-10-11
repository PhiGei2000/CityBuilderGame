#pragma once
#include "coordinateTransform.hpp"

#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

struct TerrainArea {
    /// @brief Position of the south east corner in normalized world grid coords
    glm::ivec2 position;
    /// @brief Size of the area
    glm::uvec2 size;

    /// @brief Creates a terrain area with a size of 0
    inline TerrainArea()
        : position(0), size(0) {
    }

    /// @brief Creates a terrain area with the given parameters
    /// @param position One corner of the area
    /// @param size The size of the area relative to the corner
    inline TerrainArea(const glm::ivec2& position, const glm::ivec2& size) {
        this->position = glm::min(position, position + size);
        this->size = glm::max(position, position + size) - this->position;
    }

    /// @brief Returns the intersection of the two specified areas
    /// @param t1 First area
    /// @param t2 Second area
    /// @return The intersection
    static inline TerrainArea getIntersection(const TerrainArea& t1, const TerrainArea& t2) {
        const glm::ivec2 position = glm::max(t1.position, t2.position);
        const glm::ivec2 size = glm::min(t1.position + glm::ivec2(t1.size), t2.position + glm::ivec2(t2.size)) - position;

        // no intersection
        if (size.x < 0 || size.y < 0) {
            return TerrainArea();
        }

        return TerrainArea(position, size);
    }

    /// @brief Returns the part of the terrain area which intersects with the chunk at the given position
    /// @param chunk The position of the chunk
    /// @return The part of the terrain area intersecting with the given chunk
    inline TerrainArea getAreaInChunk(const glm::ivec2& chunk) const {
        const TerrainArea& chunkArea = TerrainArea(chunk * Configuration::cellsPerChunk, glm::uvec2(Configuration::cellsPerChunk));

        return getIntersection(*this, chunkArea);
    }

    /// @brief Splits the terrain area with respect to chunks
    /// @return A map containing the areas indexed by the positions of the chunks
    inline std::unordered_map<glm::ivec2, TerrainArea> getChunkAreas() const {
        std::unordered_map<glm::ivec2, TerrainArea> chunkAreas;

        const auto& [chunk, _] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);

        const TerrainArea& chunkArea = getAreaInChunk(chunk);
        chunkAreas[chunk] = chunkArea;

        if (chunkArea.size.x < size.x) {
            TerrainArea xOverlap = TerrainArea(glm::ivec2(position.x + chunkArea.size.x, position.y), glm::ivec2(size.x - chunkArea.size.x, size.y));

            const auto& overlapAreas = xOverlap.getChunkAreas();
            for (const auto& [chunkPos, area] : overlapAreas) {
                chunkAreas[chunkPos] = area;
            }
        }

        if (chunkArea.size.y < size.y) {
            TerrainArea yOverlap = TerrainArea(glm::ivec2(position.x, position.y + chunkArea.size.y), glm::ivec2(chunkArea.size.x, size.y - chunkArea.size.y));

            const auto& overlapAreas = yOverlap.getChunkAreas();
            for (const auto& [chunkPos, area] : overlapAreas) {
                chunkAreas[chunkPos] = area;
            }
        }

        return chunkAreas;
    }

    /// @brief Shifts the area by the specified offset. Does not change the size of the area
    /// @param offset The offset
    /// @return The shifted area
    inline TerrainArea operator+(const glm::ivec2& offset) const {
        return TerrainArea(offset + position, size);
    }

    inline bool operator==(const TerrainArea& other) const {
        return position == other.position && size == other.size;
    }

    inline bool operator!=(const TerrainArea& other) const {
        return !(*this == other);
    }
};

