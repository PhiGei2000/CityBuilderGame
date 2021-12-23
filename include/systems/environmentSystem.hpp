#pragma once
#include "systems/system.hpp"

class EnvironmentSystem : public System {
  protected:
    virtual void init() override;

  public:
    EnvironmentSystem(Game* game);

    virtual void update(int dt) override;
};
