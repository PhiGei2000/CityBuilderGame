#pragma once
#include "systems/system.hpp"
#include "events/events.hpp"

struct MeshComponent;
struct TransformationComponent;

class RenderSystem : public System {
  protected:    
    unsigned int uboCamera;
    unsigned int uboLight;

    void init() override;

    void renderMesh(const MeshComponent& mesh, const glm::mat4& model) const;        

    void onCameraUpdated(CameraUpdateEvent& event) const;
    void onEntityMoved(EntityMoveEvent& event) const;

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};