#include "rendering/window.hpp"

#include "rendering/shader.hpp"

#include <iostream>

namespace trafficSimulation {
    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);        
    }

    void Window::createWindow(int width, int height, const std::string& title) {
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();

            exit(-1);
        }

        glfwMakeContextCurrent(window);

        glewInit();
    }

    void Window::render(World* world) {
        const ResourceManager& resources = world->getResourceManager();
        Geometry* streets = new Geometry();
        Shader* streetShader = resources.getResource<Shader>("StreetShader");
        Camera& camera = world->getCamera();        

        while (!glfwWindowShouldClose(window)) {
            // rendering
            if (world->geometryChanged()) {
                GeometryData data = world->getVertices();
                streets->fillBuffers(data);
            }

            glClearColor(0.200f, 0.412f, 0.118f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            streetShader->use();

            const glm::mat4& view = camera.getViewMatrix();
            const glm::mat4& projection = camera.getProjectionMatrix();
            streetShader->setMatrix4("view", view);
            streetShader->setMatrix4("projection", projection);

            streets->draw();

            // check and call events and swap the buffers
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }
} // namespace trafficSimulation