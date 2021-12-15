#pragma once
#include "systems/system.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace trafficSimulation {
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    namespace systems {
        class RenderSystem : public System {
          protected:
            GLFWwindow* window;
            entt::entity cameraEntity;

            void init() override;

          public:
            RenderSystem(Application* app);

            void update(int dt) override;
        };
    } // namespace systems
} // namespace trafficSimulation