/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "events/events.hpp"
#include "systems/system.hpp"

#include "components/buildingComponent.hpp"
#include "components/instancedMeshComponent.hpp"
#include "components/meshComponent.hpp"
#include "components/roadMeshComponent.hpp"
#include "components/transformationComponent.hpp"
#include "rendering/shadowBuffer.hpp"
#include "resources/roadPack.hpp"

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
        GameState gameState = game->getState();

        registry.view<MeshComponent, TransformationComponent>(exclude)
            .each([&](auto entity, const MeshComponent& mesh, const TransformationComponent& transform) {
                shader->setMatrix4("model", transform.transform);

                // TODO: Optimize this
                if (registry.all_of<BuildingComponent>(entity)) {
                    const BuildingComponent& building = registry.get<BuildingComponent>(entity);
                    if (building.preview && gameState != GameState::BUILD_MODE) {
                        return;
                    }

                    shader->setBool("preview", building.preview);
                }
                else {
                    shader->setBool("preview", false);
                }

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
                mesh.mesh->renderInstanced<TransformationComponent>(shader, mesh.instanceBuffer);
            });

        registry.view<MultiInstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const MultiInstancedMeshComponent& mesh, const TransformationComponent& transform) {
                shader->setMatrix4("model", transform.transform);

                for (const auto& [name, instances] : mesh.transforms) {
                    mesh.mesh->renderObjectInstanced<TransformationComponent>(shader, name, instances.instanceBuffer);
                }
            });

        // registry.view<RoadMeshComponent, TransformationComponent>(exclude).each([&](const RoadMeshComponent& road, const TransformationComponent& transform) {
        //     shader->setMatrix4("model", transform.transform);

        //     for (const auto& [typeID, tiles] : road.roadMeshes) {
        //         const std::string& roadPackName = getRoadTypeName(typeID);
        //         const RoadPackPtr& pack = resourceManager.getResource<RoadPack>(roadPackName);

        //         for (const auto& [tileType, instances] : tiles) {
        //             pack->roadGeometries.renderObjectInstanced<RoadRenderData>(shader, tileType, instances.instanceBuffer);
        //         }

        //     }
        // });
    }

    void updateLightBuffer(const LightComponent& sunLight, const CameraComponent& component) const;

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};
