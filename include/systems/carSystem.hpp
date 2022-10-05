#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/direction.hpp"

struct CarComponent;
struct TransformationComponent;
struct VelocityComponent;

class CarSystem : public System {
  protected:
    void init() override;

    static void updateCar(CarComponent& carComponent, TransformationComponent& transform, VelocityComponent& velocity);

  public:
    CarSystem(Game* game);

    void update(float dt) override;

    const entt::entity spawnCar(const glm::vec3& position, Direction drivingDirection) const;

    void handleBuildEvent(BuildEvent& e);
};
