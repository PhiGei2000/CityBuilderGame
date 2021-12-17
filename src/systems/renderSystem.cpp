#include "systems/renderSystem.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "components/components.hpp"

#include <iostream>

using namespace trafficSimulation::events;
using namespace trafficSimulation::components;

namespace trafficSimulation {
    namespace systems {

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
                registry.view<TransformationComponent, MeshComponent>()
                    .each([&](const TransformationComponent& transform, const MeshComponent& mesh) {
                        mesh.shader->use();
                        mesh.shader->setVector4("color", mesh.color);
                        
                        mesh.shader->setMatrix4("view", camera.viewMatrix);
                        mesh.shader->setMatrix4("projection", camera.projectionMatrix);

                        mesh.shader->setMatrix4("model", transform.transform);

                        mesh.geometry->draw();
                    });
            }
        }
    } // namespace systems
} // namespace trafficSimulation