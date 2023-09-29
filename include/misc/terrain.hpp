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

class Terrain {
  protected:
    Game* game;

  public:
    Terrain(Game* game);

    std::unordered_map<glm::ivec2, entt::entity> chunkEntities;

    int getTerrainHeight(const glm::ivec2& position) const;
    float getTerrainHeight(const glm::vec2& position) const;

    TerrainSurfaceTypes getSurfaceType(const glm::vec2& position) const;
};