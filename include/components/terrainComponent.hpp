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
#include "component.hpp"

#include "misc/quadtree.hpp"
#include "misc/terrain.hpp"

#include "misc/utility.hpp"

#include <ostream>

struct TerrainData {
    float terrainHeights[4];
    TerrainSurfaceTypes surfaceType;

    TerrainData()
        : surfaceType(TerrainSurfaceTypes::GRASS) {
        terrainHeights[0] = 0;
        terrainHeights[1] = 0;
        terrainHeights[2] = 0;
        terrainHeights[3] = 0;
    }

    TerrainData(float height, TerrainSurfaceTypes surfaceType)
        : surfaceType(surfaceType) {
        terrainHeights[0] = height;
        terrainHeights[1] = height;
        terrainHeights[2] = height;
        terrainHeights[3] = height;
    }

    TerrainData(float heights[4], TerrainSurfaceTypes surfaceType)
        : surfaceType(surfaceType) {
        terrainHeights[0] = heights[0];
        terrainHeights[1] = heights[1];
        terrainHeights[2] = heights[2];
        terrainHeights[3] = heights[3];
    }

    bool water() const {
        for (int i = 0; i < 4; i++) {
            if (terrainHeights[i] < 0.0f) {
                return true;
            }
        }

        return false;
    }

    inline bool operator==(const TerrainData& other) const {
        return terrainHeights[0] == other.terrainHeights[0] && terrainHeights[1] == other.terrainHeights[1] && terrainHeights[2] == other.terrainHeights[2] && terrainHeights[3] == other.terrainHeights[3] && surfaceType == other.surfaceType;
    }
};

struct TerrainComponent : public AssignableComponent {
    /// @brief A 2d array of height values for each cell
    // float** heightValues;
    /// @brief A 2d array of the surface types
    // TerrainSurfaceTypes** surfaceTypes;
    /// @brief True if the mesh is generated
    bool meshGenerated = false;

    Quadtree<TerrainData> terrain;

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        int cellsPerDirection = Configuration::chunkSize / Configuration::cellSize;

        TerrainComponent& newTerrain = registry.emplace<TerrainComponent>(entity);
        newTerrain.terrain = terrain;
    }
};
