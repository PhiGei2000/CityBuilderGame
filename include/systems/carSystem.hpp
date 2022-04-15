#pragma once

#include "system.hpp"

#include "events/events.hpp"
#include "misc/direction.hpp"

#include <glm/gtx/quaternion.hpp>

class CarSystem : public System {
  protected:
    struct Movement {
        glm::vec3 linearVelocity;
        glm::vec3 angularVelocity;
    };

    enum class Turns {
        FULL,
        LEFT,
        STAIGHT,
        RIGHT
    };

    void init() override;

    entt::entity streetEntity;

    std::vector<entt::entity> cars;

    void spawnCar();

    static glm::vec2 getDestination(Direction direction, const glm::ivec2& nodePosition);

    static Direction getTurnEndDirection(Direction startDirection, Turns turns);

    static glm::vec2 getTurnDestination(Direction startDirection, const glm::vec2& start, Turns turn);

    std::vector<Turns> getPossibleTurns(Direction direction, const glm::vec2& position) const;

    static glm::vec3 getAngularVelocity(Turns turn);

    static glm::quat getCarRotation(Direction drivingDirection);

    void handlePositionEvent(const PositionEvent& e) const;

  public:
    CarSystem(Game* game);

    void update(float dt) override;
};
