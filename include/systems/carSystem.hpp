#pragma once

#include "system.hpp"

#include "misc/direction.hpp"

class CarSystem : public System {
  protected:
    struct Movement {
      glm::vec3 linearVelocity;
      glm::vec3 angularVelocity;
    };

    enum class Turns {
      LEFT, RIGHT, FULL, STAIGHT
    };

    void init() override;

    entt::entity streetEntity;

    std::vector<entt::entity> cars;

    void spawnCar();

    static glm::vec2 getDestination(Direction direction, const glm::ivec2& nodePosition);

    static Movement getMovement(const glm::vec2& start, const glm::vec2& end);
  public:
    CarSystem(Game* game);

    void update(float dt) override;
};
