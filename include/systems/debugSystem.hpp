#pragma once
#include "system.hpp"

#include "events/events.hpp"
#include "rendering/shader.hpp"

#include <glm/glm.hpp>

class DebugSystem : public System {
  protected:    
    unsigned int vbo, vao;

    entt::entity cameraEntity;
    entt::entity debugEntity;

    void init() override;

  public:
    DebugSystem(Game* game);

    void update(float dt);

    void handleKeyEvent(const KeyEvent& e);
};
