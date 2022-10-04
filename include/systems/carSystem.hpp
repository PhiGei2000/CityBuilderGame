#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/direction.hpp"

class CarSystem : public System {
  protected:
    void init() override;

  public:
    CarSystem(Game* game);

    void update(float dt) override;

    const entt::entity spawnCar(const glm::vec3& position, Direction drivingDirection) const;

    void handleBuildEvent(BuildEvent& e);
};
