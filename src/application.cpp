#include "application.hpp"

#include "systems/renderSystem.hpp"
#include "systems/system.hpp"

#include "rendering/shader.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include <iostream>

using namespace trafficSimulation::systems;
using namespace trafficSimulation::events;

namespace trafficSimulation {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);

        Application* app = (Application*)glfwGetWindowUserPointer(window);
        app->raiseEvent(FramebufferSizeEvent{app, width, height});
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        app->raiseEvent(KeyEvent{app, key, scancode, action, mods});
    }

    void cursorPos_callback(GLFWwindow* window, double x, double y) {            
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        app->raiseEvent(MouseMoveEvent{app, (float)x, (float)y, app->lastCursorPos.x, app->lastCursorPos.y});

        app->lastCursorPos = glm::vec2(x, y);
    }

    void Application::init() {
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
        glfwSetCursorPosCallback(window, cursorPos_callback);

        glfwMakeContextCurrent(window);

        glewInit();

        systems.push_back(new RenderSystem(this));
    }

    void Application::loadResources() {
        resourceManager.setResource<Shader>("MESH_SHADER", new Shader("res/mesh.vert", "res/mesh.frag"));
    }

    Application::Application() {
        init();
    }

    void Application::run() {
        glfwSetTime(0);

        while (!stopRequested) {
            int dt = (int)(glfwGetTime() * 1000);
            glfwSetTime(0);
            for (System* system : systems) {
                system->update(dt);
            }
        }
    }

    entt::registry& Application::getRegistry() {
        return registry;
    }

    entt::dispatcher& Application::getEventDispatcher() {
        return eventDispatcher;
    }

    ResourceManager& Application::getResourceManager() {
        return resourceManager;
    }

    template<typename Event>
    void Application::raiseEvent(const Event& event) {
        eventDispatcher.trigger<Event>(event);
    }

    template void Application::raiseEvent<FramebufferSizeEvent>(const FramebufferSizeEvent&);
    template void Application::raiseEvent<KeyEvent>(const KeyEvent&);
    template void Application::raiseEvent<MouseMoveEvent>(const MouseMoveEvent&);
} // namespace trafficSimulation