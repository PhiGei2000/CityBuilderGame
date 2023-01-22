#pragma once
#include "component.hpp"

struct TerrainComponent : public AssignableComponent {
    float** heightValues;

    inline float getHeightValue(const glm::vec2 position) const {
        constexpr unsigned int cellsPerDirection = Configuration::worldSize / Configuration::gridSize;
        glm::uvec2 gridPos = glm::floor(1.0f / Configuration::gridSize * position);

        float h0 = heightValues[gridPos.x][gridPos.y];
        float h1 = (gridPos.x + 1) < cellsPerDirection ? heightValues[gridPos.x + 1][gridPos.y] : h0;
        float h2 = (gridPos.y + 1) < cellsPerDirection ? heightValues[gridPos.x][gridPos.y + 1] : h0;
        float h3 = ((gridPos.x + 1) < cellsPerDirection && (gridPos.y + 1) < cellsPerDirection) ? heightValues[gridPos.x + 1][gridPos.y + 1] : h0;

        glm::vec2 cellPos = position - glm::vec2(gridPos);
        float x0 = h0 + cellPos.x * (h1 - h0);
        float x1 = h2 + cellPos.x * (h3 - h2);

        float height = x0 + cellPos.y * (x1 - x0);
        return height;
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        int cellsPerDirection = Configuration::worldSize / Configuration::gridSize;

        TerrainComponent& newTerrain = registry.emplace<TerrainComponent>(entity);
        newTerrain.heightValues = new float*[cellsPerDirection];

        for (int x = 0; x < cellsPerDirection; x++) {
            newTerrain.heightValues[x] = new float[cellsPerDirection];
            for (int y = 0; y < cellsPerDirection; y++) {
                newTerrain.heightValues[x][y] = heightValues[x][y];
            }
        }
    }
};