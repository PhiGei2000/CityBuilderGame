#include "misc/terrain.hpp"

#include "components/terrainComponent.hpp"
#include "game.hpp"

#include "misc/coordinateTransform.hpp"

Terrain::Terrain(Game* game)
    : game(game) {
}

int Terrain::getTerrainHeight(const glm::ivec2& position) const {
    const glm::ivec2& chunkPosition = utility::gridToChunkCoords(position);
    const glm::ivec2& chunkGridCoords = 1.0f / Configuration::cellSize * glm::vec2(position - Configuration::chunkSize * chunkPosition);

    const entt::entity entity = chunkEntities.at(chunkPosition);
    const TerrainComponent& terrainComponent = game->getRegistry().get<TerrainComponent>(entity);

    return terrainComponent.heightValues[chunkGridCoords.x][chunkGridCoords.y];
}

float Terrain::getTerrainHeight(const glm::vec2& position) const {
    const glm::ivec2& cellCoords = glm::floor(position);

    // linear interpolation of the height values
    float h0 = getTerrainHeight(cellCoords);
    float h1 = getTerrainHeight(cellCoords + glm::ivec2(1, 0));
    float h2 = getTerrainHeight(cellCoords + glm::ivec2(0, 1));
    float h3 = getTerrainHeight(cellCoords + glm::ivec2(1, 1));

    const glm::vec2& cellPos = position - static_cast<float>(Configuration::cellSize) * glm::floor(1.0f / Configuration::cellSize * glm::vec2(cellCoords));
    float x0 = h0 + cellPos.x * (h1 - h0) / static_cast<float>(Configuration::cellSize);
    float x1 = h2 + cellPos.x * (h3 - h2) / static_cast<float>(Configuration::cellSize);

    float height = x0 + cellPos.y * (x1 - x0) / static_cast<float>(Configuration::cellSize);
    return height;
}

TerrainSurfaceTypes Terrain::getSurfaceType(const glm::vec2& position) const {
    constexpr glm::vec2 offsets[9] = {
        static_cast<float>(Configuration::cellSize) * glm::vec2(0, 0),
        static_cast<float>(Configuration::cellSize) * glm::vec2(1, 0),
        static_cast<float>(Configuration::cellSize) * glm::vec2(0, 1),
        static_cast<float>(Configuration::cellSize) * glm::vec2(1, 1),
        static_cast<float>(Configuration::cellSize) * glm::vec2(-1, 0),
        static_cast<float>(Configuration::cellSize) * glm::vec2(0, -1),
        static_cast<float>(Configuration::cellSize) * glm::vec2(-1, -1),
        static_cast<float>(Configuration::cellSize) * glm::vec2(-1, 1),
        static_cast<float>(Configuration::cellSize) * glm::vec2(1, -1),
    };

    for (int i = 0; i < 4; i++) {
        if (getTerrainHeight(position + offsets[i]) < 0.0f) {
            return TerrainSurfaceTypes::WATER;
        }
    }

    int waterCount = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 4; j++) {
            if (getTerrainHeight(position + offsets[j] + offsets[i]) < 0) {
                waterCount++;
                j = 4;
            }
        }
    }

    if (waterCount >= 1) {
        return TerrainSurfaceTypes::BEACH;
    }

    return TerrainSurfaceTypes::GRASS;
}