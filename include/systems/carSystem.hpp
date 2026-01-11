#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/direction.hpp"
#include "misc/roads/graph.hpp"

struct CarComponent;
struct TransformationComponent;
struct VelocityComponent;

class CarSystem : public System {
  protected:
    unsigned int carsCount = 0;
    void init() override;

    static void updateCar(CarComponent& carComponent, TransformationComponent& transform, VelocityComponent& velocity);
    void updateCarPath(CarComponent& carComponent) const;

    void spawnCars();

    const entt::entity spawnCar(const RoadPath& path);
    const entt::entity spawnCar(const glm::vec3& position, float roation);

  public:
    CarSystem(Game* game);

    void update(float dt) override;

    void handleBuildEvent(BuildEvent& e);
};
