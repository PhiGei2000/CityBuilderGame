#include "systems/renderSystem.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "components/components.hpp"

#include <iostream>

void RenderSystem::init() {
}

RenderSystem::RenderSystem(Application* app)
    : System(app) {
    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();
}

void RenderSystem::update(int dt) {
    const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

    if (!glfwWindowShouldClose(app->getWindow())) {
        glClearColor(0.0f, 0.698f, 0.894f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        registry.view<TransformationComponent, MeshComponent>()
            .each([&](const TransformationComponent& transform, const MeshComponent& mesh) {
                mesh.texture->use(0);

                mesh.shader->use();
                mesh.shader->setInt("diffuse", 0);
                
                mesh.shader->setMatrix4("view", camera.viewMatrix);
                mesh.shader->setMatrix4("projection", camera.projectionMatrix);
                mesh.shader->setMatrix4("model", transform.transform);

                mesh.geometry->draw();
            });
    }
}