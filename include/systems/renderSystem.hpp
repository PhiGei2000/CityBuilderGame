#pragma once
#include "systems/system.hpp"

class RenderSystem : public System {
  protected:
    entt::entity cameraEntity;

    void init() override;

  public:
    RenderSystem(Application* app);

    void update(int dt) override;
};