#pragma once
#include <vector>

#include "game.hpp"
#include "gui/gui.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class System;
struct KeyEvent;
struct FramebufferSizeEvent;
struct MouseButtonEvent;
struct MouseMoveEvent;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursorPos_callback(GLFWwindow* window, double x, double y);

class Application {
  private:
    GLFWwindow* window;

    Gui* gui = nullptr;
    Game* game = nullptr;

    bool stopRequested = false;

    friend void cursorPos_callback(GLFWwindow*, double, double);
    glm::vec2 lastCursorPos = glm::vec2(400.0f, 300.0f);

    void init();

  public:
    bool gamePaused = false;

    Application();

    void run();

    GLFWwindow* getWindow() const;

    void onKeyEvent(const KeyEvent& e);
    void onFramebufferSizeEvent(const FramebufferSizeEvent& e);
    void onMouseMoveEvent(const MouseMoveEvent& e);
    void onMouseButtonEvent(const MouseButtonEvent& e);
};