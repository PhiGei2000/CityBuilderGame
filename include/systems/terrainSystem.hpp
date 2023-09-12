#pragma once
#include "system.hpp"

#include <queue>

#include <glm/glm.hpp>

struct TerrainComponent;
struct MeshComponent;

class TerrainSystem : public System {
  protected:
    glm::ivec2 lastChunk = glm::ivec2(-INT_MAX);

    std::queue<glm::ivec2> chunksToGenerate;
    std::queue<glm::ivec2> chunksToCreateMesh;

    void init() override;

    void generateTerrain(TerrainComponent& terrain, const glm::ivec2& chunkPosition) const;

    void generateTerrainMesh(const TerrainComponent& terrain, MeshComponent& mesh) const;    

  public:
    TerrainSystem(Game* game);

    void update(float dt) override;
};
