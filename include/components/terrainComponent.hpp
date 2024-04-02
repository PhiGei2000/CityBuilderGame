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

#include "misc/utility.hpp"

#include <ostream>

struct TerrainComponent : public AssignableComponent {
    /// @brief A 2-d array of height values for each cell
    float heightValues[Configuration::cellsPerChunk + 1][Configuration::cellsPerChunk + 1];
    /// @brief True if the mesh is generated
    bool meshGenerated = false;

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        int cellsPerDirection = Configuration::chunkSize / Configuration::cellSize;

        TerrainComponent& newTerrain = registry.emplace<TerrainComponent>(entity);
        for (int x = 0; x < cellsPerDirection + 1; x++) {
            for (int y = 0; y < cellsPerDirection + 1; y++) {
                newTerrain.heightValues[x][y] = heightValues[x][y];
            }
        }
    }
};
