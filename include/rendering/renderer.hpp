#pragma once
#include "geometry.hpp"
#include "vertex.hpp"
#include "world.hpp"
#include "rendering/camera.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <vector>

namespace trafficSimulation {
    class Renderer {
      private:
        GLFWwindow* window;        
        std::map<std::string, Geometry> geometries;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

      public:
        static void init();

        void createWindow(int width, int height, const std::string& title);

        void render(World* world);
    };
} // namespace trafficSimulation