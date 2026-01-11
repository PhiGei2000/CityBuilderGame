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

    const static std::array<std::string, 2> treeNames;

    struct SunLight {
        glm::vec3 ambient = 0.4f * glm::vec3(1.0f, 1.0f, 220.0f / 250.0f);
        glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 220.0f / 250.0f);
        glm::vec3 specular = 0.8f * glm::vec3(1.0f, 1.0f, 220.0f / 250.0f);
    } sunLight;

    void updateDayNightCycle(float dt, TransformationComponent& sunTransform, SunLightComponent& sunLight) const;

    void destroyEntities();

    void clearCells();

  public:
    EnvironmentSystem(Game* game);

    virtual void update(float dt) override;

    void handleBuildEvent(const BuildEvent& e);

    void handleChunkCreatedEvent(const ChunkCreatedEvent& e) const;
};
