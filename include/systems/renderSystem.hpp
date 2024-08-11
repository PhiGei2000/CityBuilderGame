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
struct ShaderProgram;

class RenderSystem : public System {
  protected:
    unsigned int uboCamera;
    unsigned int uboLight;

    ShadowBuffer shadowBuffer;
#if DEBUG
    ShadowMapRenderer shadowMapRenderer;
#endif

    unsigned int cameraWidth;

    ShaderPtr shadowShader;

    glm::vec4 skyColor = glm::vec4(1.0f, 1.0f, 220.0f / 255.0f, 1.0f);

    void init() override;

    void onCameraUpdated(CameraUpdateEvent& event) const;
    void onEntityMoved(EntityMoveEvent& event) const;

    template<typename... T>
    inline void renderScene(entt::exclude_t<T...> exclude = {}) const {
        GameState gameState = game->getState();

        registry.view<MeshComponent, TransformationComponent>(exclude)
            .each([&](auto entity, const MeshComponent& mesh, const TransformationComponent& transform) {
                MeshRenderData renderData = {transform.transform};

                // TODO: Optimize this
                if (registry.all_of<BuildingComponent>(entity)) {
                    const BuildingComponent& building = registry.get<BuildingComponent>(entity);
                    if (building.preview && gameState != GameState::BUILD_MODE) {
                        return;
                    }

                    renderData.preview = building.preview;
                }

                mesh.mesh->render(renderData);
            });

        registry.view<InstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const InstancedMeshComponent& mesh, const TransformationComponent& transform) {
                MeshRenderData renderData = {transform.transform};
                mesh.mesh->renderInstanced<TransformationComponent>(renderData, mesh.instanceBuffer);
            });

        registry.view<MultiInstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const MultiInstancedMeshComponent& mesh, const TransformationComponent& transform) {
                MeshRenderData renderData = {transform.transform};

                for (const auto& [name, instances] : mesh.transforms) {
                    mesh.mesh->renderObjectInstanced<TransformationComponent>(name, renderData, instances.instanceBuffer);
                }
            });

        registry.view<RoadMeshComponent, TransformationComponent>(exclude).each([&](const RoadMeshComponent& road, const TransformationComponent& transform) {
            MeshRenderData renderData = {transform.transform};

            for (const auto& [typeID, tiles] : road.roadMeshes) {
                const std::string& roadPackName = getRoadTypeName(typeID);
                const RoadPackPtr& pack = resourceManager.getResource<RoadPack>(roadPackName);

                for (const auto& [tileType, instances] : tiles) {
                    pack->roadGeometries.renderObjectInstanced<glm::mat4>(tileType, renderData, instances.instanceBuffer);
                }
            }

#if DEBUG
            if (game->debugMode) {
                ShaderProgram* roadDebugPointsShader = resourceManager.getResource<Shader>("ROAD_DEBUG_POINTS_SHADER")->defaultShader;
                ShaderProgram* roadDebugLinesShader = resourceManager.getResource<Shader>("ROAD_DEBUG_LINES_SHADER")->defaultShader;

                roadDebugLinesShader->use();
                roadDebugLinesShader->setMatrix4("model", transform.transform);

                road.graphDebugMesh->draw();

                glEnable(GL_PROGRAM_POINT_SIZE);
                roadDebugPointsShader->use();
                roadDebugPointsShader->setMatrix4("model", transform.transform);

                road.graphDebugMesh->draw();
                glDisable(GL_PROGRAM_POINT_SIZE);
            }
#endif
        });
    }

    template<typename... T>
    inline void renderSceneShadows(entt::exclude_t<T...> exclude = {}) const {
        GameState gameState = game->getState();

        registry.view<MeshComponent, TransformationComponent>(exclude)
            .each([&](auto entity, const MeshComponent& mesh, const TransformationComponent& transform) {
                MeshRenderData renderData = {transform.transform};

                // TODO: Optimize this
                if (registry.all_of<BuildingComponent>(entity)) {
                    const BuildingComponent& building = registry.get<BuildingComponent>(entity);
                    if (building.preview && gameState != GameState::BUILD_MODE) {
                        return;
                    }

                    renderData.preview = building.preview;
                }

                mesh.mesh->render(renderData, shadowShader.get());
            });

        registry.view<InstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const InstancedMeshComponent& mesh, const TransformationComponent& transform) {
                MeshRenderData renderData = {transform.transform};
                mesh.mesh->renderInstanced<TransformationComponent>(renderData, mesh.instanceBuffer, shadowShader.get());
            });

        registry.view<MultiInstancedMeshComponent, TransformationComponent>(exclude)
            .each([&](const MultiInstancedMeshComponent& mesh, const TransformationComponent& transform) {
                MeshRenderData renderData = {transform.transform};

                for (const auto& [name, instances] : mesh.transforms) {
                    mesh.mesh->renderObjectInstanced<TransformationComponent>(name, renderData, instances.instanceBuffer, shadowShader.get());
                }
            });

        registry.view<RoadMeshComponent, TransformationComponent>(exclude).each([&](const RoadMeshComponent& road, const TransformationComponent& transform) {
            MeshRenderData renderData = {transform.transform};

            for (const auto& [typeID, tiles] : road.roadMeshes) {
                const std::string& roadPackName = getRoadTypeName(typeID);
                const RoadPackPtr& pack = resourceManager.getResource<RoadPack>(roadPackName);

                for (const auto& [tileType, instances] : tiles) {
                    pack->roadGeometries.renderObjectInstanced<glm::mat4>(tileType, renderData, instances.instanceBuffer, shadowShader.get());
                }
            }
        });
    }

    void updateLightBuffer(const LightComponent& sunLight, const CameraComponent& component) const;

  public:
    RenderSystem(Game* app);

    void update(float dt) override;
};
