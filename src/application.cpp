#include "application.hpp"

namespace trafficSimulation {
    void Application::init() {
        if (!glfwInit()) {
            std::cerr << "failed to intialize GLFW!" << std::endl;
            exit(1);
        }
    }

    Application::Application() {
        init();

        window.createWindow(800, 600, "Traffic Simulation");

        world.init();
    }

    void Application::run() {
        window.render(&world);
    }
} // namespace trafficSimulation