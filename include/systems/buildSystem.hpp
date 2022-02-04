#pragma once
#include "system.hpp"

#include <glm/glm.hpp>

struct MouseButtonEvent;

class BuildSystem : public System {
  protected:
    virtual void init() override;

    entt::entity cameraEntity;
    entt::entity buildMarkerEntity;

    enum class BuildSystemState
    {
        IDLE,
        STREET
    };

    BuildSystemState state = BuildSystemState::IDLE;

    glm::ivec2 getGridPos(const glm::vec2& mousePos) const;

  public:
    BuildSystem(Game* game);

    virtual void update(float dt) override;

    void handleMouseButtonEvent(const MouseButtonEvent& e);
};
