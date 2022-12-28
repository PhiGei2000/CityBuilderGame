#pragma once
#include "events/events.hpp"
#include "systems/system.hpp"

#include "rendering/shadowBuffer.hpp"

struct MeshComponent;
struct LightComponent;
struct CameraComponent;
struct TransformationComponent;
struct Shader;

class RenderSystem : public System {
  protected:
    unsigned int uboCamera;
    unsigned int uboLight;

    ShadowBuffer shadowBuffer;

    unsigned int cameraWidth;

    std::shared_ptr<Shader> depthShader;

    void init() override;

    void onCameraUpdated(CameraUpdateEvent& event) const;
    void onEntityMoved(EntityMoveEvent& event) const;
    void renderScene(Shader* shader = nullptr) const;

    void updateLightBuffer(const LightComponent& sunLight) const;

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};