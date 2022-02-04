#pragma once
#include "systems/system.hpp"

class RenderSystem : public System {
  protected:
    entt::entity cameraEntity;
    unsigned int uboMatrices;

    void init() override;

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};