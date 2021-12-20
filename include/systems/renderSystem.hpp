#pragma once
#include "systems/system.hpp"

class RenderSystem : public System {
  protected:
    entt::entity cameraEntity;

    void init() override;

  public:
    RenderSystem(Game* app);

    void update(int dt) override;
};