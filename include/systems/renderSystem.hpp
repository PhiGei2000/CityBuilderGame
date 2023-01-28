#pragma once
#include "events/events.hpp"
#include "systems/system.hpp"

#include "components/instancedMeshComponent.hpp"
#include "components/meshComponent.hpp"
#include "components/transformationComponent.hpp"
#include "rendering/shadowBuffer.hpp"

#if DEBUG
#include "rendering/debug/shadowMapRenderer.hpp"
#endif

struct LightComponent;
struct CameraComponent;
struct Shader;

class RenderSystem : public System {
  protected:
    unsigned int uboCamera;
    unsigned int uboLight;

    ShadowBuffer shadowBuffer;
#if DEBUG
    ShadowMapRenderer shadowMapRenderer;
#endif

    unsigned int cameraWidth;

    std::shared_ptr<Shader> shadowShader;

    void init() override;

    void onCameraUpdated(CameraUpdateEvent& event) const;
    void onEntityMoved(EntityMoveEvent& event) const;

    template<typename... T>
    inline void renderScene(entt::exclude_t<T...> exclude = {}, Shader* shader = nullptr) const {
        registry.view<MeshComponent, TransformationComponent>(exclude)
            .each([&](const MeshComponent& mesh, const TransformationComponent& transform) {
                mesh.mesh->render(transform.transform, shader);
            });

        ShaderPtr instancedShader = resourceManager.getResource<Shader>("MESH_INSTANCED_SHADER");
        registry.view<InstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const InstancedMeshComponent& mesh, const TransformationComponent& transform) {
                mesh.mesh->renderInstanced(mesh.offsets.size(), transform.transform, instancedShader.get());
            });
    }

    void updateLightBuffer(const LightComponent& sunLight, const CameraComponent& component) const;

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};