#include "systems/renderSystem.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "components/components.hpp"

#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

void RenderSystem::init() {
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

RenderSystem::RenderSystem(Game* game)
    : System(game) {
    init();
    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();
}

void RenderSystem::update(float dt) {
    const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.projectionMatrix));

    registry.view<TransformationComponent, MeshComponent>(entt::exclude<BuildMarkerComponent>)
        .each([&](const TransformationComponent& transform, const MeshComponent& mesh) {
            mesh.texture->use(0);

            mesh.shader->use();
            mesh.shader->setInt("diffuse", 0);
            mesh.shader->setVector3("light.color", glm::vec3(0.9f));
            mesh.shader->setVector3("light.direction", glm::normalize(glm::vec3(1, -1, 1)));
            mesh.shader->setVector3("viewPos", cameraTransform.position);

            mesh.shader->setMatrix4("model", transform.transform);

            mesh.geometry->draw();
        });

    if (game->getState() == GameState::BUILD_MODE) {
        registry.view<TransformationComponent, MeshComponent, BuildMarkerComponent>()
            .each([&](const TransformationComponent& transform, const MeshComponent& mesh, const BuildMarkerComponent& buildMarker) {
                mesh.texture->use(0);

                mesh.shader->use();
                mesh.shader->setInt("diffuse", 0);
                mesh.shader->setVector3("light.color", glm::vec3(0.9f));
                mesh.shader->setVector3("light.direction", glm::normalize(glm::vec3(1, 1, 1)));

                mesh.shader->setMatrix4("model", transform.transform);

                mesh.geometry->draw();
            });
    }
}