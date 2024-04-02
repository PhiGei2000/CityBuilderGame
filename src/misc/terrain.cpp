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

int Terrain::getTerrainHeight(const glm::ivec2& position) const {
    const auto& [chunk, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);

    const entt::entity entity = chunkEntities.at(chunk);
    const TerrainComponent& terrainComponent = game->getRegistry().get<TerrainComponent>(entity);

    return terrainComponent.heightValues[pos.x][pos.y];
}

float Terrain::getTerrainHeight(const glm::vec2& position) const {
    const glm::ivec2& cellCoords = glm::floor(position);

    // linear interpolation of the height values
    float h0 = getTerrainHeight(cellCoords);
    float h1 = getTerrainHeight(cellCoords + glm::ivec2(1, 0));
    float h2 = getTerrainHeight(cellCoords + glm::ivec2(0, 1));
    float h3 = getTerrainHeight(cellCoords + glm::ivec2(1, 1));

    const glm::vec2& cellPos = position - glm::vec2(cellCoords);
    float x0 = h0 + cellPos.x * (h1 - h0);
    float x1 = h2 + cellPos.x * (h3 - h2);

    float height = x0 + cellPos.y * (x1 - x0);
    return height;
}

void Terrain::setTerrainHeight(const glm::ivec2& position, float height) const {
    const auto& [chunk, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);

    const entt::entity entity = chunkEntities.at(chunk);
    TerrainComponent& terrain = game->getRegistry().get<TerrainComponent>(entity);

    terrain.heightValues[pos.x][pos.y] = height;
}

TerrainSurfaceTypes Terrain::getSurfaceType(const glm::vec2& position) const {
    constexpr glm::vec2 offsets[9] = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(0, 1),
        glm::vec2(1, 1),
        glm::vec2(-1, 0),
        glm::vec2(0, -1),
        glm::vec2(-1, -1),
        glm::vec2(-1, 1),
        glm::vec2(1, -1),
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

bool Terrain::positionValid(const glm::vec2& position) const {
    const auto& [chunk, _] = utility::normalizedWorldGridToNormalizedChunkGridCoords(position);

    return chunkEntities.contains(chunk);
}

TerrainSurfaceGeometry Terrain::getGeometry(const glm::ivec2& cell) const {
    float h0 = getTerrainHeight(cell);
    float h1 = getTerrainHeight(cell + glm::ivec2(1, 0));
    float h2 = getTerrainHeight(cell + glm::ivec2(0, 1));
    float h3 = getTerrainHeight(cell + glm::ivec2(1, 1));

    if (h0 == h1 && h1 == h2 && h2 == h3) {
        return TerrainSurfaceGeometry::FLAT;
    }

    if ((h0 == h1 && h2 == h3) || (h1 == h2 && h0 == h3)) {
        return TerrainSurfaceGeometry::FLAT_TILTED;
    }

    if (h1 == h2 && h2 == h3) {
        return h0 < h1 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
    }
    else if (h0 == h2 && h2 == h3) {
        return h1 < h0 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
    }
    else if (h0 == h1 && h1 == h3) {
        return h2 < h0 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
    }
    else if (h0 == h1 && h1 == h2) {
        return h3 < h0 ? TerrainSurfaceGeometry::DIAGONAL_TILTED_BOTTOM : TerrainSurfaceGeometry::INNER_CORNER;
    }

    throw std::runtime_error("Terrain surface type is invalid");
}
