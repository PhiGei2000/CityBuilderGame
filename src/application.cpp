#include "application.hpp"

#include "modelLoader.hpp"

#include "systems/cameraSystem.hpp"
#include "systems/renderSystem.hpp"
#include "systems/system.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"

#include "components/components.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    Application* app = (Application*)glfwGetWindowUserPointer(window);
    app->onFramebufferSizeEvent(FramebufferSizeEvent{width, height});
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    app->onKeyEvent(KeyEvent{key, scancode, action, mods});
}

void cursorPos_callback(GLFWwindow* window, double x, double y) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    app->onMouseMoveEvent(MouseMoveEvent{(float)x, (float)y, app->lastCursorPos.x, app->lastCursorPos.y});

    app->lastCursorPos = glm::vec2(x, y);
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    app->onMouseButtonEvent(MouseButtonEvent{(float)x, (float)y, button, action, mods});
}

void Application::init() {
    if (!glfwInit()) {
        std::cerr << "failed to intialize GLFW!" << std::endl;
        exit(1);
    }    

#if DEBUG
    window = glfwCreateWindow(800, 600, "City Building Game", NULL, NULL);
#else    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    
    int x, y, width, height;
    glfwGetMonitorWorkarea(monitor, &x, &y, &width, &height);

    window = glfwCreateWindow(width, height, "City Building Game", monitor, NULL);    
#endif

    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;

        const char* buffer;
        int code = glfwGetError(&buffer);

        std::cerr << "Error Code " << code << std::endl;
        std::cerr << buffer << std::endl;
        glfwTerminate();

        exit(1);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursorPos_callback);
    glfwSetMouseButtonCallback(window, mouseButton_callback);

    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glewInit();

    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // init gui
    gui = new Gui(this);
    gui->setScreenSize(800, 600);

    // init game
    game = new Game(this);
}

Application::Application() {
    init();
}

void Application::run() {
    glfwSetTime(0);
    float lastTime = 0;

    while (!stopRequested) {
        float currentTime = (float)glfwGetTime();
        float dt = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.7f, 0.877f, 0.917f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game->update(dt);

        gui->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        stopRequested |= glfwWindowShouldClose(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::stop() {
    stopRequested = true;
}

GLFWwindow* Application::getWindow() const {
    return window;
}

void Application::onKeyEvent(const KeyEvent& e) {
    gui->handleKeyEvent(e);

    game->raiseEvent(e);
}

void Application::onFramebufferSizeEvent(const FramebufferSizeEvent& e) {
    gui->setScreenSize(e.width, e.height);

    game->raiseEvent(e);
}

void Application::onMouseMoveEvent(const MouseMoveEvent& e) {
    game->raiseEvent(e);
}

void Application::onMouseButtonEvent(const MouseButtonEvent& e) {
    game->raiseEvent(e);

    gui->handleMouseButtonEvent(e);
}
