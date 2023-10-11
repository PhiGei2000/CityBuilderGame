#pragma once
#include <unordered_map>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

class Game;

enum class TerrainSurfaceTypes {
    GRASS,
    WATER,
    BEACH
};

enum class TerrainSurfaceGeometry {
    FLAT,
    FLAT_TILTED,
    OUTER_CORNER, // unused
    INNER_CORNER,
    DIAGONAL_TILTED_BOTTOM,
    DIAGONAL_TILTED_TOP // unused
};

class Terrain {
  protected:
    Game* game;

  public:
    Terrain(Game* game);

    std::unordered_map<glm::ivec2, entt::entity> chunkEntities;

    /// @brief Returns the terrain height of the cell at the specified position
    /// @param position The position in normalized world grid coords
    /// @return The terrain height at the specified position
    int getTerrainHeight(const glm::ivec2& position) const;
    /// @brief Returns the interpolated terrain height at the specified position
    /// @param position The position in normalized world grid coords
    /// @return The interpolated terrain height at the specified position
    float getTerrainHeight(const glm::vec2& position) const;

    /// @brief Sets the terrain height at the specified position
    /// @param position The position in normalized world grid coords
    /// @param height The height
    void setTerrainHeight(const glm::ivec2& position, float height) const;

    /// @brief Determines the surface type (grass, water, beach, ...) at the specified position
    /// @param position The position in normalized world grid coords
    /// @return The surface type at the specified position
    TerrainSurfaceTypes getSurfaceType(const glm::vec2& position) const;

    /// @brief Determines if the chunk at the specified position is generated.
    /// @param position The position in normalized world grid coordinates
    /// @return True if the chunk is already generated
    bool positionValid(const glm::vec2& position) const;

    /// @brief Returns the geometry type of the given cell
    /// @param cell The position of the cell in normalized world grid coordinates
    /// @return The surface geometry
    TerrainSurfaceGeometry getGeometry(const glm::ivec2& cell) const;
};