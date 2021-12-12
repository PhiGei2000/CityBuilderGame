#include "rendering/renderer.hpp"

#include "rendering/shader.hpp"

#include <iostream>

namespace trafficSimulation {
    void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);        
    }

    void Renderer::init() {
        if (!glfwInit()) {
            std::cerr << "failed to intialize GLFW!" << std::endl;
            exit(1);
        }
    }

    void Renderer::createWindow(int width, int height, const std::string& title) {
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();

            exit(-1);
        }

        glfwMakeContextCurrent(window);

        glewInit();
    }

    void Renderer::render(World* world) {
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