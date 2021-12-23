#pragma once
#include "system.hpp"

#include <glm/glm.hpp>

class BuildSystem : public System {
  protected:
    virtual void init() override;

    entt::entity cameraEntity;
    entt::entity buildMarker;

    glm::ivec2 getGridPos(const glm::vec2& mousePos) const;

    glm::ivec2 buildMarkerPos;
  public:
    static constexpr float gridSize = 10.0f;

    BuildSystem(Game* game);

    virtual void update(int dt) override;
};
