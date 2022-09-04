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
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // sun light buffer    
    glGenBuffers(1, &uboLight);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLight);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);        
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

RenderSystem::RenderSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<CameraUpdateEvent>()
        .connect<&RenderSystem::onCameraUpdated>(*this);
}

void RenderSystem::renderMesh(const MeshComponent& mesh, const glm::mat4& model) const {
    mesh.material->diffuse->use(0);

    mesh.shader->use();
    mesh.shader->setInt("material.diffuse", 0);
    mesh.shader->setFloat("material.ambientStrength", mesh.material->ambient);
    mesh.shader->setFloat("material.specularStrenght", mesh.material->specularStrenght);
    mesh.shader->setFloat("material.shininess", mesh.material->shininess);

    mesh.shader->setVector3("light.color", glm::vec3(0.9f));
    mesh.shader->setVector3("light.direction", glm::normalize(glm::vec3(1, -1, 1)));

    // mesh.shader->setVector3("cameraTarget", cameraTransform.position + 5.0f * cameraFront);

    mesh.shader->setMatrix4("model", model);

    mesh.geometry->draw();
}

void RenderSystem::onCameraUpdated(CameraUpdateEvent& event) const {
    const entt::entity& cameraEntity = event.entity;
    const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

    // set camera buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboCamera);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.projectionMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(cameraTransform.position));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(camera.front));
}

void RenderSystem::onEntityMoved(EntityMoveEvent& event) const {
    
}

void RenderSystem::update(float dt) {    
    registry.view<TransformationComponent, MeshComponent>(entt::exclude<BuildMarkerComponent>)
        .each([&](const TransformationComponent& transform, const MeshComponent& mesh) {
            renderMesh(mesh, transform.transform);
        });

    registry.view<TransformationComponent, MultiMeshComponent>(entt::exclude<DebugComponent>)
        .each([&](const TransformationComponent& transform, const MultiMeshComponent& multiMesh) {
            for (const auto& [id, mesh] : multiMesh.meshes) {
                renderMesh(mesh, transform.transform);
            }
        });

    if (game->getState() == GameState::BUILD_MODE) {
        registry.view<TransformationComponent, MeshComponent, BuildMarkerComponent>()
            .each([&](const TransformationComponent& transform, const MeshComponent& mesh, const BuildMarkerComponent& buildMarker) {
                renderMesh(mesh, transform.transform);
            });
    }

    if (game->debugMode) {
        entt::entity debugEntity = registry.view<DebugComponent>().front();

        // const DebugComponent& debug = registry.get<DebugComponent>(debugEntity);
        const MultiMeshComponent& multiMesh = registry.get<MultiMeshComponent>(debugEntity);
        const TransformationComponent& transform = registry.get<TransformationComponent>(debugEntity);

        for (const auto& [id, mesh] : multiMesh.meshes) {
            renderMesh(mesh, transform.transform);
        }
    }
}