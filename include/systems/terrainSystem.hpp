#pragma once
#include "system.hpp"

#include "misc/terrainArea.hpp"

#include <queue>

#include <glm/glm.hpp>
#include <noise/noise.h>

struct TerrainComponent;
struct MeshComponent;
struct BuildEvent;
struct TextureAtlas;
struct Vertex;

class TerrainSystem : public System {
  protected:
    noise::module::Perlin terrainNoise;
    static constexpr float noiseScaleFactor = 15.0f;

    static const TextureAtlas atlas;

    glm::ivec2 lastChunk = glm::ivec2(-INT_MAX);

    std::queue<glm::ivec2> chunksToGenerate;
    std::queue<glm::ivec2> chunksToCreateMesh;
    std::queue<TerrainArea> areasToUpdateMesh;

    void init() override;

    void generateTerrain(TerrainComponent& terrain, const glm::ivec2& chunkPosition) const;

    void generateTerrainMesh(const glm::ivec2& chunkPosition, MeshComponent& mesh) const;

    unsigned int generateTerrainQuadMesh(const glm::ivec2& position, const glm::ivec2& chunkPosition, std::vector<Vertex>& terrainVertices) const;
    unsigned int generateWaterQuadMesh(const glm::ivec2& position, const glm::ivec2& chunkPosition, std::vector<Vertex>& waterVertices) const;

    void updateTerrainMesh(const TerrainArea& area) const;
    void updateTerrainMesh(const TerrainArea& area, MeshComponent& mesh) const;

  public:
    TerrainSystem(Game* game);

    void update(float dt) override;

    void handleBuildEvent(const BuildEvent& event);
};
