#pragma once
#include "component.hpp"

#include "misc/utility.hpp"

struct TerrainComponent : public AssignableComponent {
  private:
    inline static glm::uvec2 getGridPos(const glm::vec2& position) {
        assert(utility::inRange<float>(position.x, 0, Configuration::worldSize));
        assert(utility::inRange<float>(position.y, 0, Configuration::worldSize));

        return glm::floor(1.0f / Configuration::gridSize * position);
    }

  public:
    float** heightValues;

    inline float getHeightValue(const glm::vec2& position) const {
        const glm::uvec2 gridPos = getGridPos(position);

        // linear interpolation of the height values
        float h0 = heightValues[gridPos.x][gridPos.y];
        float h1 = heightValues[gridPos.x + 1][gridPos.y];
        float h2 = heightValues[gridPos.x][gridPos.y + 1];
        float h3 = heightValues[gridPos.x + 1][gridPos.y + 1];

        glm::vec2 cellPos = position - static_cast<float>(Configuration::gridSize) * glm::vec2(gridPos);
        float x0 = h0 + cellPos.x * (h1 - h0) / static_cast<float>(Configuration::gridSize);
        float x1 = h2 + cellPos.x * (h3 - h2) / static_cast<float>(Configuration::gridSize);

        float height = x0 + cellPos.y * (x1 - x0) / static_cast<float>(Configuration::gridSize);
        return height;
    }

    inline glm::vec3 getSurfaceNormal(const glm::vec2& position) const {
        glm::uvec2 gridPos = getGridPos(position);

        float h0 = heightValues[gridPos.x][gridPos.y];
        float h1 = heightValues[gridPos.x + 1][gridPos.y];
        float h2 = heightValues[gridPos.x][gridPos.y + 1];

        const glm::vec3 e1 = glm::vec3(Configuration::gridSize, h1 - h0, 0.0f);
        const glm::vec3 e2 = glm::vec3(0.0f, h2 - h0, Configuration::gridSize);

        return glm::normalize(glm::cross(e2, e1));
    }

    inline bool isWater(const glm::vec2& position) const {
        constexpr unsigned int cellsPerDirection = Configuration::worldSize / Configuration::gridSize;
        glm::uvec2 gridPos = getGridPos(position);

        for (unsigned int x = 0; x <= 1; x++) {
            for (unsigned int y = 0; y <= 1; y++) {
                if (gridPos.x + x < cellsPerDirection && gridPos.y + y < cellsPerDirection) {
                    if (heightValues[gridPos.x + x][gridPos.y + y] < 0) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        int cellsPerDirection = Configuration::worldSize / Configuration::gridSize;

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