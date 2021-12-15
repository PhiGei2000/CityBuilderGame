#include "application.hpp"

#include "systems/renderSystem.hpp"
#include "systems/system.hpp"

#include "rendering/shader.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"

using namespace trafficSimulation::systems;
using namespace trafficSimulation::events;

namespace trafficSimulation {
    void Application::init() {
        systems.push_back(new RenderSystem(this));
    }

    void Application::loadResources() {
        resourceManager.setResource<Shader>("MESH_SHADER", new Shader("res/mesh.vert", "res/mesh.frag"));
    }

    Application::Application() {
    }

    void Application::run() {
        glfwSetTime(0);

        while(!stopRequested) {
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

    template void Application::raiseEvent<FramebufferSizeEvent>(const FramebufferSizeEvent& event);
    template void Application::raiseEvent<KeyEvent>(const KeyEvent& event);
} // namespace trafficSimulation