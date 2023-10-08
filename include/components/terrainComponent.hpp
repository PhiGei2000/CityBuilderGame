#pragma once
#include "component.hpp"

#include "misc/utility.hpp"

#include <ostream>

struct TerrainComponent : public AssignableComponent {
    /// @brief A 2-d array of height values for each cell
    float** heightValues;
    /// @brief True if the mesh is generated
    bool meshGenerated = false;

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        int cellsPerDirection = Configuration::chunkSize / Configuration::cellSize;

        TerrainComponent& newTerrain = registry.emplace<TerrainComponent>(entity);
        newTerrain.heightValues = new float*[cellsPerDirection + 1];

        for (int x = 0; x < cellsPerDirection + 1; x++) {
            newTerrain.heightValues[x] = new float[cellsPerDirection + 1];
            for (int y = 0; y < cellsPerDirection + 1; y++) {
                newTerrain.heightValues[x][y] = heightValues[x][y];
            }
        }
    }
};