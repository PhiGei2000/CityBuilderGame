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

    /// @brief Returns the part of the terrain area which intersects with the chunk at the given position
    /// @param chunk The position of the chunk
    /// @return The part of the terrain area intersecting with the given chunk
    inline TerrainArea getAreaInChunk(const glm::ivec2& chunk) const {
        const auto& [posChunk, posInsideChunk] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);
        if (posChunk != chunk) {
            return TerrainArea(glm::ivec2(0), glm::ivec2(0));
        }

        glm::uvec2 chunkAreaSize;

        // trim size if the area reaches to the neighbour chunk
        if (posInsideChunk.x + size.x >= Configuration::chunkSize) {
            chunkAreaSize.x = Configuration::cellsPerChunk - posInsideChunk.x;
        }
        else {
            chunkAreaSize.x = size.x;
        }

        if (posInsideChunk.y + size.y >= Configuration::chunkSize) {
            chunkAreaSize.y = Configuration::cellsPerChunk - posInsideChunk.y;
        }
        else {
            chunkAreaSize.y = size.y;
        }

        return TerrainArea(position, chunkAreaSize);
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
};