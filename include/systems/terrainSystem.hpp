#pragma once
#include "system.hpp"

struct TerrainComponent;
struct MeshComponent;

class TerrainSystem : public System {
  protected:
    void init() override;

    void generateTerrain(TerrainComponent& terrain) const;

    void generateTerrainMesh(const TerrainComponent& terrain, MeshComponent& mesh) const;    

  public:
    TerrainSystem(Game* game);

    void update(float dt);
};
