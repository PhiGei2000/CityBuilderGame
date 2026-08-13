#pragma once
#include "component.hpp"

#include "misc/quadtree.hpp"
#include "misc/terrain.hpp"

#include "misc/utility.hpp"

#include <ostream>

struct TerrainData {
    /// @brief Height values of the cell corners
    ///
    /// 1 3
    ///
    /// 0 2
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

    TerrainData(const float heights[4], TerrainSurfaceTypes surfaceType)
        : surfaceType(surfaceType) {
        terrainHeights[0] = heights[0];
        terrainHeights[1] = heights[1];
        terrainHeights[2] = heights[2];
        terrainHeights[3] = heights[3];
    }

    TerrainData(const TerrainData& other)
        : TerrainData(other.terrainHeights, other.surfaceType) {
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
    /// @brief True if the mesh is generated
    bool meshGenerated = false;

    /// @brief True if the mesh is outdated
    bool meshOutdated = false;

    Quadtree<TerrainData> terrain;

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        int cellsPerDirection = Configuration::chunkSize / Configuration::cellSize;

        TerrainComponent& newTerrain = registry.emplace<TerrainComponent>(entity);
        newTerrain.terrain = terrain;
    }

    inline const TerrainData& getCellHeights(const glm::ivec2& position) const {
        return terrain.getNode(position).value;
    }
};
