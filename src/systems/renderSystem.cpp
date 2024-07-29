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
#include "systems/renderSystem.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "components/components.hpp"

#include "misc/configuration.hpp"

#include <iostream>
#include <numeric>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

void RenderSystem::init() {
    // camera buffer
    glGenBuffers(1, &uboCamera);
    glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    CameraUpdateEvent e{game->camera, true, true, true};
    onCameraUpdated(e);

    // sun light buffer
    glGenBuffers(1, &uboLight);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLight);
    glBufferData(GL_UNIFORM_BUFFER, 2 * Configuration::SHADOW_CASCADE_COUNT * sizeof(glm::mat4) + (4 + Configuration::SHADOW_CASCADE_COUNT) * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // shadow rendering
    shadowShader = resourceManager.getResource<Shader>("SHADOW_SHADER");
}

RenderSystem::RenderSystem(Game* game)
    : System(game) {
    init();

    // connect event handlers
    eventDispatcher.sink<CameraUpdateEvent>()
        .connect<&RenderSystem::onCameraUpdated>(*this);

    eventDispatcher.sink<EntityMoveEvent>()
        .connect<&RenderSystem::onEntityMoved>(*this);
}

void RenderSystem::onCameraUpdated(CameraUpdateEvent& event) const {
    // get components and calculate camera target
    const entt::entity& cameraEntity = event.entity;
    const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

    const glm::vec3& cameraTarget = cameraTransform.position + camera.front;

    // bind camera buffer to location 1
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboCamera);
    // view matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.viewMatrix));
    // projection matrix
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.projectionMatrix));
    // camera position
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(cameraTransform.position));
    // camera target
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(cameraTarget));

    if (game->sun != entt::null) {
        SunLightComponent& sunLight = registry.get<SunLightComponent>(game->sun);
        sunLight.calculateLightMatrices(camera);
        updateLightBuffer(sunLight, camera);
    }
}

void RenderSystem::onEntityMoved(EntityMoveEvent& event) const {
    // update sun uniform buffer
    if (event.entity == game->sun) {
        const CameraComponent& camera = registry.get<CameraComponent>(game->camera);
        SunLightComponent& sunLight = registry.get<SunLightComponent>(game->sun);

        // recalculate matrices and update light buffer
        sunLight.calculateLightMatrices(camera);
        updateLightBuffer(sunLight, camera);
    }
}

void RenderSystem::updateLightBuffer(const LightComponent& sunLight, const CameraComponent& camera) const {
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboLight);
    for (int i = 0; i < Configuration::SHADOW_CASCADE_COUNT; i++) {
        // light view
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(sunLight.lightView[i]));
        // light projection
        glBufferSubData(GL_UNIFORM_BUFFER, (Configuration::SHADOW_CASCADE_COUNT + i) * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(sunLight.lightProjection[i]));
        // cascade far planes
        float value = (camera.far - camera.near) * Configuration::CASCADE_FAR_PLANE_FACTORS[i] + camera.near;
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * Configuration::SHADOW_CASCADE_COUNT * sizeof(glm::mat4) + (4 + i) * sizeof(glm::vec4), sizeof(float), &value);
    }
    // light direction
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * Configuration::SHADOW_CASCADE_COUNT * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(sunLight.direction));
    // light ambient
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * Configuration::SHADOW_CASCADE_COUNT * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(sunLight.ambient));
    // light diffuse
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * Configuration::SHADOW_CASCADE_COUNT * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(sunLight.diffuse));
    // light specular
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * Configuration::SHADOW_CASCADE_COUNT * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(sunLight.specular));
}

void RenderSystem::update(float dt) {
    // shadows
    shadowBuffer.use();
    glClear(GL_DEPTH_BUFFER_BIT);

    glCullFace(GL_FRONT);
    renderSceneShadows(entt::exclude<DebugComponent>);

    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const CameraComponent& camera = registry.get<CameraComponent>(game->camera);
    glViewport(0, 0, camera.width, camera.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if DEBUG
    const entt::entity& debugEntity = registry.view<DebugComponent>().front();
    const DebugComponent& debugComponent = registry.get<DebugComponent>(debugEntity);
    if (debugComponent.mode == DebugMode::SHADOW_MAPS) {
        glDisable(GL_CULL_FACE);
        shadowMapRenderer.render(shadowBuffer);
        glEnable(GL_CULL_FACE);

        return;
    }
#endif

    shadowBuffer.bindTextures();

    renderScene(entt::exclude<DebugComponent>);

    if (game->getState() == GameState::BUILD_MODE) {
        registry.view<TransformationComponent, MeshComponent, BuildingComponent>()
            .each([&](const TransformationComponent& transform, const MeshComponent& mesh, const BuildingComponent& buildMarker) {
                MeshRenderData renderData = {transform.transform};
                mesh.mesh->render(renderData);
            });
    }

    if (game->debugMode) {
        entt::entity debugEntity = registry.view<DebugComponent>().front();

        // const DebugComponent& debug = registry.get<DebugComponent>(debugEntity);
        const MeshComponent& mesh = registry.get<MeshComponent>(debugEntity);
        const TransformationComponent& transform = registry.get<TransformationComponent>(debugEntity);

        MeshRenderData renderData = {transform.transform};

        mesh.mesh->render(renderData);
    }
}
