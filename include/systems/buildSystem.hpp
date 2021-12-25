#pragma once
#include "system.hpp"

#include <glm/glm.hpp>

struct MouseButtonEvent;

class BuildSystem : public System {
  protected:
    virtual void init() override;

    entt::entity cameraEntity;
    entt::entity buildMarkerEntity;

    glm::ivec2 getGridPos(const glm::vec2& mousePos) const;

    bool** usedSpace;
  public:
    static constexpr int gridSize = 5.0f;
    static constexpr int worldSize = 100.0f;

    BuildSystem(Game* game);

    virtual void update(int dt) override;

    void handleMouseButtonEvent(const MouseButtonEvent& e);
};
