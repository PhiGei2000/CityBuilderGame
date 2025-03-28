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
#include "misc/terrain.hpp"

#include "components/terrainComponent.hpp"
#include "game.hpp"

#include "misc/coordinateTransform.hpp"

#include <stdexcept>

Terrain::Terrain(Game* game)
    : game(game) {
}

float Terrain::getTerrainHeight(const glm::ivec2& position) const {
    const auto& [chunk, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);

    const entt::entity entity = chunkEntities.at(chunk);
    const TerrainComponent& terrainComponent = game->getRegistry().get<TerrainComponent>(entity);

    return terrainComponent.terrain[pos].value.terrainHeights[0];
}

std::array<float, 4> Terrain::getTerrainCellHeights(const glm::ivec2& position) const {
    const auto& [chunk, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);

    const entt::entity entity = chunkEntities.at(chunk);
    const TerrainComponent& terrainComponent = game->getRegistry().get<TerrainComponent>(entity);
    const TerrainData& data = terrainComponent.terrain[glm::ivec2(pos)].value;

    return std::array<float, 4>({
        data.terrainHeights[0],
        data.terrainHeights[1],
        data.terrainHeights[2],
        data.terrainHeights[3],
    });
}

float Terrain::getTerrainHeight(const glm::vec2& position) const {
    const glm::ivec2& cellCoords = glm::floor(position);

    // linear interpolation of the height values
    auto [h0, h1, h2, h3] = getTerrainCellHeights(cellCoords);

    const glm::vec2& cellPos = position - glm::vec2(cellCoords);
    float x0 = h0 + cellPos.x * (h1 - h0);
    float x1 = h2 + cellPos.x * (h3 - h2);

    float height = x0 + cellPos.y * (x1 - x0);
    return height;
}

void Terrain::setTerrainHeight(const glm::ivec2& position, float height) const {
    const glm::ivec2 positions[4] = {
        position,
        position + glm::ivec2(-1, 0),
        position + glm::ivec2(0, -1),
        position + glm::ivec2(-1, -1),
    };

    for (int i = 0; i < 4; i++) {
        const auto& [chunk, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(positions[i]);

        const entt::entity entity = chunkEntities.at(chunk);
        TerrainComponent& terrain = game->getRegistry().get<TerrainComponent>(entity);

        if (terrain.terrain.getNode(pos).value.terrainHeights[i] != height) {
            terrain.terrain.createLeaf(pos).value.terrainHeights[i] = height;
            terrain.meshOutdated = true;
        }
    }
}

void Terrain::setTerrainCellHeights(const glm::ivec2& position, const std::array<float, 4>& cellHeights) const {
    setTerrainHeight(position, cellHeights[0]);
    setTerrainHeight(position + glm::ivec2(1, 0), cellHeights[1]);
    setTerrainHeight(position + glm::ivec2(0, 1), cellHeights[2]);
    setTerrainHeight(position + glm::ivec2(1, 1), cellHeights[3]);
}

TerrainSurfaceTypes Terrain::getSurfaceType(const glm::vec2& position) const {
    const auto& [chunk, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);

    const entt::entity entity = chunkEntities.at(chunk);
    const TerrainComponent& terrainComponent = game->getRegistry().get<TerrainComponent>(entity);

    return terrainComponent.terrain[glm::ivec2(glm::floor(pos))].value.surfaceType;
}

bool Terrain::chunkLoaded(const glm::ivec2& position) const {
    return chunkEntities.contains(position);
}

const std::vector<glm::ivec2> Terrain::getLoadedChunks() const {
    std::vector<glm::ivec2> chunks(chunkEntities.size());
    std::transform(chunkEntities.begin(), chunkEntities.end(),
                   std::inserter(chunks, chunks.begin()),
                   [](const std::pair<glm::ivec2, entt::entity>& pair) {
                       return pair.first;
                   });

    return chunks;
}

bool Terrain::positionValid(const glm::vec2& position) const {
    const auto& [chunk, _] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);
    return chunkLoaded(chunk);
}

// TerrainSurfaceGeometry Terrain::getGeometry(const glm::ivec2& cell) const {
//     auto [h0, h1, h2, h3] = getTerrainCellHeights(cell);

//     if (h0 == h1 && h1 == h2 && h2 == h3) {
//         return TerrainSurfaceGeometry::FLAT;
//     }

//     if ((h0 == h1 && h2 == h3) || (h1 == h2 && h0 == h3)) {
//         return TerrainSurfaceGeometry::FLAT_TILTED;
//     }

//     if (h1 == h2 && h2 == h3) {
//         return h0 < h1 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
//     }
//     else if (h0 == h2 && h2 == h3) {
//         return h1 < h0 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
//     }
//     else if (h0 == h1 && h1 == h3) {
//         return h2 < h0 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
//     }
//     else if (h0 == h1 && h1 == h2) {
//         return h3 < h0 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
//     }

//     throw std::runtime_error("Terrain surface type is invalid");
// }

std::vector<Triangle> Terrain::getSurfaceTriangles(const glm::ivec2& cell) const {
    const int x = cell.x, y = cell.y;

    auto [h0, h1, h2, h3] = getTerrainCellHeights(cell);
    glm::vec3 positions[4] = {
        glm::vec3(x * Configuration::cellSize, h0, y * Configuration::cellSize),
        glm::vec3((x + 1) * Configuration::cellSize, h1, y * Configuration::cellSize),
        glm::vec3(x * Configuration::cellSize, h2, (y + 1) * Configuration::cellSize),
        glm::vec3((x + 1) * Configuration::cellSize, h3, (y + 1) * Configuration::cellSize),
    };

    if (glm::length(positions[0] - positions[3]) < glm::length(positions[1] - positions[2])) {
        return {Triangle(positions[0], positions[1], positions[3]),
                Triangle(positions[0], positions[3], positions[2])};
    }
    else {
        return {Triangle(positions[0], positions[1], positions[2]),
                Triangle(positions[1], positions[3], positions[2])};
    }
}
