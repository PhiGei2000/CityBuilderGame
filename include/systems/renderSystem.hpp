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
    std::shared_ptr<Shader> instancedShadowShader;

    void init() override;

    void onCameraUpdated(CameraUpdateEvent& event) const;
    void onEntityMoved(EntityMoveEvent& event) const;

    template<typename... T>
    inline void renderScene(ShaderPtr shader, entt::exclude_t<T...> exclude = {}) const {
        shader->use();
        shader->setInt("shadowMaps", ShadowBuffer::depthMapOffset);

        registry.view<MeshComponent, TransformationComponent>(exclude)
            .each([&](const MeshComponent& mesh, const TransformationComponent& transform) {
                shader->setMatrix4("model", transform.transform);
                mesh.mesh->render(shader);
            });
    }

    template<typename... T>
    inline void renderSceneInstanced(ShaderPtr shader, entt::exclude_t<T...> exclude = {}) const {
        shader->use();
        shader->setInt("shadowMaps", ShadowBuffer::depthMapOffset);

        registry.view<InstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const InstancedMeshComponent& mesh, const TransformationComponent& transform) {
                shader->setMatrix4("model", transform.transform);
                mesh.mesh->renderInstanced(shader, mesh.instanceBuffer);
            });

        registry.view<MultiInstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const MultiInstancedMeshComponent& mesh, const TransformationComponent& transform) {
                shader->setMatrix4("model", transform.transform);

                for (const auto& [name, instances] : mesh.transforms) {
                    mesh.mesh->renderObjectInstanced(shader, name, instances.instanceBuffer);
                }
            });
    }

    void updateLightBuffer(const LightComponent& sunLight, const CameraComponent& component) const;

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};