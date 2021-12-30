#pragma once
#include "systems/system.hpp"

#include <queue>

struct BuildEvent;

class EnvironmentSystem : public System {
  protected:
    virtual void init() override;

    std::queue<entt::entity> entitiesToDestroy;

  public:
    EnvironmentSystem(Game* game);

    virtual void update(float dt) override;

    void handleBuildEvent(const BuildEvent& e);
};
