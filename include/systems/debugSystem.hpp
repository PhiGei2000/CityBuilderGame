#pragma once
#include "system.hpp"

#include "events/events.hpp"
#include "rendering/shader.hpp"

#include <glm/glm.hpp>

class DebugSystem : public System {
  protected:
    static constexpr glm::vec3 axis[3] = {
        glm::vec3{1, 0, 0},
        glm::vec3{0, 1, 0},
        glm::vec3{0, 0, 1}};

    Shader* axisShader;
    unsigned int vbo, vao;

    entt::entity cameraEntity;

    void init() override;

  public:
    DebugSystem(Game* game);

    void update(float dt);

    void handleKeyEvent(const KeyEvent& e);
};
