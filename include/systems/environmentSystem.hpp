#pragma once
#include "systems/system.hpp"

#include <queue>

struct BuildEvent;
struct ChunkCreatedEvent;
struct TransformationComponent;
struct SunLightComponent;
struct TerrainComponent;

class EnvironmentSystem : public System {
  protected:
    virtual void init() override;

    std::queue<entt::entity> entitiesToDestroy;
    std::queue<glm::ivec2> cellsToClear;

    void updateDayNightCycle(float dt, TransformationComponent& sunTransform, SunLightComponent& sunLight) const;

  public:
    EnvironmentSystem(Game* game);

    virtual void update(float dt) override;

    void handleBuildEvent(const BuildEvent& e);

    void handleChunkCreatedEvent(const ChunkCreatedEvent& e) const;
};
