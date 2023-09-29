#pragma once
#include "system.hpp"

#include <queue>

#include <glm/glm.hpp>
#include <noise/noise.h>

struct TerrainComponent;
struct MeshComponent;

class TerrainSystem : public System {
  protected:
    noise::module::Perlin terrainNoise;
    static constexpr float noiseScaleFactor = 15.0f;

    glm::ivec2 lastChunk = glm::ivec2(-INT_MAX);

    std::queue<glm::ivec2> chunksToGenerate;
    std::queue<glm::ivec2> chunksToCreateMesh;

    void init() override;

    void generateTerrain(TerrainComponent& terrain, const glm::ivec2& chunkPosition) const;

    void generateTerrainMesh(const TerrainComponent& terrain, const glm::ivec2& chunkPosition, MeshComponent& mesh) const;

  public:
    TerrainSystem(Game* game);

    void update(float dt) override;
};
