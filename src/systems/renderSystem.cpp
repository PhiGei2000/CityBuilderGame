#include "systems/renderSystem.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"

#include "components/components.hpp"

#include <iostream>

using namespace trafficSimulation::events;
using namespace trafficSimulation::components;

namespace trafficSimulation {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);

        Application* app = (Application*)glfwGetWindowUserPointer(window);
        app->raiseEvent(FramebufferSizeEvent{width, height});
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        app->raiseEvent(KeyEvent{key, scancode, action, mods});
    }

    namespace systems {

        void RenderSystem::init() {
            if (!glfwInit()) {
                std::cerr << "failed to intialize GLFW!" << std::endl;
                exit(1);
            }

            window = glfwCreateWindow(800, 600, "Traffic Simulation", NULL, NULL);

            if (window == NULL) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();

                exit(1);
            }

            glfwSetWindowUserPointer(window, this);

            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwSetKeyCallback(window, key_callback);

            glfwMakeContextCurrent(window);

            glewInit();
        }

        RenderSystem::RenderSystem(Application* app)
            : System(app) {
                cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();
        }

        void RenderSystem::update(int dt) {
            const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
            const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);
            
            if (!glfwWindowShouldClose(window)) {                
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