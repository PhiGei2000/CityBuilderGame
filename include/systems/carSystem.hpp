#pragma once
#include "system.hpp"

#include "misc/direction.hpp"

class CarSystem : public System {
  protected:
    void init() override;

  public:
    CarSystem(Game* game);

    void update(float dt) override;

    void spawnCar(const glm::vec3& position, Direction drivingDirection) const;
};
