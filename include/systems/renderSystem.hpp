#pragma once
#include "systems/system.hpp"

struct MeshComponent;
struct TransformationComponent;

class RenderSystem : public System {
  protected:
    entt::entity cameraEntity;
    unsigned int uboMatrices;

    void init() override;

    void renderMesh(const MeshComponent& mesh, const glm::mat4& model, const TransformationComponent& cameraTransform, const glm::vec3& cameraFront) const;    

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};