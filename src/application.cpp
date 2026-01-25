#include "application.hpp"

#include "systems/system.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"

#include "components/components.hpp"

#include "events/framebufferSizeEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "gui/menus/menus.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    Application* app = (Application*)glfwGetWindowUserPointer(window);
    FramebufferSizeEvent event = FramebufferSizeEvent(width, height);
    app->onFramebufferSizeEvent(event);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    KeyEvent event = KeyEvent(key, scancode, action, mods);
    app->onKeyEvent(event);
}

void cursorPos_callback(GLFWwindow* window, double x, double y) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    MouseMoveEvent event = MouseMoveEvent{(float)x, (float)y, app->lastCursorPos.x, app->lastCursorPos.y};
    app->onMouseMoveEvent(event);

    app->lastCursorPos = glm::vec2(x, y);
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    MouseButtonEvent event = MouseButtonEvent{(float)x, (float)y, button, action, mods};
    app->onMouseButtonEvent(event);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);

    MouseScrollEvent event = MouseScrollEvent((float)xoffset, (float)yoffset);
    app->onMouseScrollEvent(event);
}

void Application::init() {
    if (!glfwInit()) {
        std::cerr << "failed to intialize GLFW!" << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    // anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    int x, y, width, height;
    GLFWmonitor* monitor = NULL;
#if DEBUG
    width = 800;
    height = 600;
#else
    monitor = glfwGetPrimaryMonitor();

    glfwGetMonitorWorkarea(monitor, &x, &y, &width, &height);

#endif
    window = glfwCreateWindow(width, height, "City Building Game", monitor, NULL);

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
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glewInit();

    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // init game
    game = new Game(this);

    // init gui
    gui = new Gui(this, 800, 600);
    gui->addMenu(static_cast<int>(GameMenus::PAUSE_MENU), new PauseMenu(gui));
    gui->addMenu(static_cast<int>(GameMenus::OPTIONS_MENU), new OptionsMenu(gui));
    gui->addMenu(static_cast<int>(GameMenus::BUILD_MENU), new BuildMenu(gui));
#if DEBUG
    gui->addMenu(static_cast<int>(GameMenus::DEBUG_PANEL), new DebugPanel(gui));
#endif

    FramebufferSizeEvent framebufferSizeEvent{width, height};
    game->raiseEvent<true>(framebufferSizeEvent);

    gui->init();
}

Application::Application() {
    init();
}

void Application::run() {
    glfwSetTime(0);
    float lastTime = 0;

    while (!stopRequested) {
        float currentTime = (float)glfwGetTime();
        updateTime = currentTime - lastTime;
        lastTime = currentTime;

        // glClearColor(0.7f, 0.877f, 0.917f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game->update(updateTime);

        gui->update();
        gui->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        stopRequested |= (glfwWindowShouldClose(window) != 0);
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

void Application::onKeyEvent(KeyEvent& e) {
    gui->handleKeyEvent(e);

    if (!e.handled) {
        game->raiseEvent<false>(e);
    }
}

void Application::onFramebufferSizeEvent(FramebufferSizeEvent& e) {
    gui->setScreenSize(e.width, e.height);

    game->raiseEvent<true>(e);
}

void Application::onMouseMoveEvent(MouseMoveEvent& e) {
    game->raiseEvent<false>(e);

    gui->handleMouseMoveEvent(e);
}

void Application::onMouseButtonEvent(MouseButtonEvent& e) {
    gui->handleMouseButtonEvent(e);

    if (!e.handled) {
        game->raiseEvent<false>(e);
    }
}

void Application::onMouseScrollEvent(MouseScrollEvent& e) {
    game->raiseEvent<false>(e);
}

bool Application::getMouseButton(int mouseButton) const {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

glm::vec2 Application::getMousePos() const {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    return glm::vec2{static_cast<float>(x), static_cast<float>(y)};
}