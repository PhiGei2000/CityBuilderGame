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
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 4 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // shadow depth buffer and texture
    glGenFramebuffers(1, &depthFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Configuration::SHADOW_BUFFER_WIDTH, Configuration::SHADOW_BUFFER_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthShader = resourceManager.getResource<Shader>("DEPTH_SHADER");
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
        LightComponent& sunLight = registry.get<LightComponent>(game->sun);
        sunLight.calculateLightMatrices(camera);
        updateLightBuffer(sunLight);
    }
}

void RenderSystem::onEntityMoved(EntityMoveEvent& event) const {
    // update sun uniform buffer
    if (event.entity == game->sun) {
        const CameraComponent& camera = registry.get<CameraComponent>(game->camera);
        LightComponent& sunLight = registry.get<LightComponent>(game->sun);

        sunLight.calculateLightMatrices(camera);
        updateLightBuffer(sunLight);
    }
}

void RenderSystem::updateLightBuffer(const LightComponent& sunLight) const {
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboLight);
    // light view
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(sunLight.lightView));
    // light projection
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(sunLight.lightProjection));
    // light direction
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(sunLight.direction));
    // light ambient
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(sunLight.ambient));
    // light diffuse
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(sunLight.diffuse));
    // light specular
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(sunLight.specular));
}

void RenderSystem::renderScene(Shader* shader) const {
    registry.view<TransformationComponent, MeshComponent>(entt::exclude<BuildMarkerComponent, DebugComponent>)
        .each([&](const TransformationComponent& transform, const MeshComponent& mesh) {
            mesh.mesh->render(transform.transform, shader);
        });
}

void RenderSystem::update(float dt) {
    // shadows
    glViewport(0, 0, Configuration::SHADOW_BUFFER_WIDTH, Configuration::SHADOW_BUFFER_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderScene(depthShader.get());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const CameraComponent& camera = registry.get<CameraComponent>(game->camera);
    glViewport(0, 0, camera.width, camera.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const int offset = 4;
    glActiveTexture(GL_TEXTURE0 + offset);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboLight);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 4 * sizeof(glm::vec4), sizeof(int), &offset);

    renderScene();

    if (game->getState() == GameState::BUILD_MODE) {
        registry.view<TransformationComponent, MeshComponent, BuildMarkerComponent>()
            .each([&](const TransformationComponent& transform, const MeshComponent& mesh, const BuildMarkerComponent& buildMarker) {
                mesh.mesh->render(transform.transform);
            });
    }

    if (game->debugMode) {
        entt::entity debugEntity = registry.view<DebugComponent>().front();

        // const DebugComponent& debug = registry.get<DebugComponent>(debugEntity);
        const MeshComponent& mesh = registry.get<MeshComponent>(debugEntity);
        const TransformationComponent& transform = registry.get<TransformationComponent>(debugEntity);

        mesh.mesh->render(transform.transform);
    }
}