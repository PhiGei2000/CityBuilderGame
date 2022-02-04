#pragma once

#include "system.hpp"

class CarSystem : public System {
  protected:
    void init() override;

    entt::entity streetEntity;

    std::vector<entt::entity> cars;

    void spawnCar();

  public:
    CarSystem(Game* game);

    void update(float dt) override;
};
