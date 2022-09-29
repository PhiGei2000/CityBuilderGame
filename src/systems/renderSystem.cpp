#include "systems/renderSystem.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "components/components.hpp"

#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

void RenderSystem::init() {
    // camera buffer
    glGenBuffers(1, &uboCamera);
    glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    entt::entity camera = registry.view<CameraComponent, TransformationComponent>().front();
    CameraUpdateEvent e{camera, true, true, true};
    onCameraUpdated(e);

    // sun light buffer
    glGenBuffers(1, &uboLight);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLight);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
}

void RenderSystem::onEntityMoved(EntityMoveEvent& event) const {
    // update sun uniform buffer
    if (event.entity == game->sun) {
        const LightComponent& sunLight = registry.get<LightComponent>(game->sun);

        glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboLight);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(sunLight.direction));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(sunLight.ambient));
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(sunLight.diffuse));
        glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(sunLight.specular));
    }
}

void RenderSystem::update(float dt) {
    registry.view<TransformationComponent, MeshComponent>(entt::exclude<BuildMarkerComponent, DebugComponent>)
        .each([&](const TransformationComponent& transform, const MeshComponent& mesh) {
            mesh.mesh->render(transform.transform);
        });

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