#pragma once
#include "system.hpp"

class PhysicsSystem : public System {
  private:
    void init() override;

  public:
    PhysicsSystem(Game* game);

    void update(float dt) override;
};
