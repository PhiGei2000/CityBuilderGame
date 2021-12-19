#pragma once
#include <vector>

#include "resourceManager.hpp"
#include "gui/gui.hpp"

#include <entt/entt.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class System;
struct KeyEvent;
struct FramebufferSizeEvent;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursorPos_callback(GLFWwindow* window, double x, double y);

class Application {
  private:
    std::vector<System*> systems;

    ResourceManager resourceManager;

    entt::registry registry;
    entt::dispatcher eventDispatcher;

    GLFWwindow* window;

    Gui* gui = nullptr;

    bool stopRequested = false;

    friend void cursorPos_callback(GLFWwindow*, double, double);
    glm::vec2 lastCursorPos = glm::vec2(400.0f, 300.0f);

    void init();

    void loadResources();

  public:
    bool gamePaused = false;
    
    Application();

    void run();

    entt::registry& getRegistry();
    entt::dispatcher& getEventDispatcher();
    ResourceManager& getResourceManager();
    GLFWwindow* getWindow() const;

    void onKeyEvent(const KeyEvent& e);
    void onFramebufferSizeEvent(const FramebufferSizeEvent& e);

    template<typename Event>
    void raiseEvent(const Event& args);
};