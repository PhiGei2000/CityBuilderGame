#pragma once
#include "system.hpp"

#include "events/events.hpp"
#include "rendering/shader.hpp"

#include <glm/glm.hpp>

class DebugSystem : public System {
  protected:    
    unsigned int vbo, vao;

    Geometry* axisGeo;

    entt::entity cameraEntity;
    entt::entity debugEntity;

    void init() override;

  public:
    DebugSystem(Game* game);
    ~DebugSystem();

    void update(float dt) override;

    void handleKeyEvent(const KeyEvent& e);
};
