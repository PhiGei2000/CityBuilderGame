#pragma once
#include "systems/system.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

struct CameraComponent;

class CameraSystem : public System {
  protected:
    entt::entity cameraEntity;

    float yaw = -90.0f;
    float pitch = 0.0f;

    int inputX = 0, inputZ = 0;

    virtual void init() override;

    std::pair<glm::vec3, glm::vec2> getCameraMovement(const CameraComponent& camera) const;

  public:
    CameraSystem(Game* game);

    virtual void update(float dt) override;

    void onFramebufferSize(const FramebufferSizeEvent& e);
};
