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
    app->raiseEvent(FramebufferSizeEvent{app, width, height});
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    app->raiseEvent(KeyEvent{app, key, scancode, action, mods});
}

void cursorPos_callback(GLFWwindow* window, double x, double y) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    app->raiseEvent(MouseMoveEvent{app, (float)x, (float)y, app->lastCursorPos.x, app->lastCursorPos.y});

    app->lastCursorPos = glm::vec2(x, y);
}

void Application::init() {
    if (!glfwInit()) {
        std::cerr << "failed to intialize GLFW!" << std::endl;
        exit(1);
    }

    window = glfwCreateWindow(800, 600, "Traffic Simulation", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        exit(1);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursorPos_callback);

    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glewInit();

    glEnable(GL_DEPTH_TEST);
        
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // init resources
    loadResources();

    // init systems
    systems.push_back(new CameraSystem(this));
    systems.push_back(new RenderSystem(this));

    // init gui
    gui = new Gui();
    gui->setScreenSize(800, 600);
    gui->init();
}

void Application::loadResources() {
    // models
    resourceManager.setResource<Geometry>("GROUND_GEOMETRY", ModelLoader::load("res/models/ground.obj"));

    // shaders
    resourceManager.setResource<Shader>("MESH_SHADER", new Shader("res/shaders/mesh.vert", "res/shaders/mesh.frag"));

    // textures
    resourceManager.setResource<Texture>("GROUND_TEXTURE", new Texture("res/textures/ground.png"));
    resourceManager.setResource<Texture>("STREET_TEXTURE", new Texture("res/textures/street_texture_array.png"));

    // entities
    entt::entity groundEntity = registry.create();
    registry.emplace<MeshComponent>(groundEntity,
                                    resourceManager.getResource<Geometry>("GROUND_GEOMETRY"),
                                    resourceManager.getResource<Shader>("MESH_SHADER"),
                                    resourceManager.getResource<Texture>("GROUND_TEXTURE"));

    registry.emplace<TransformationComponent>(groundEntity, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
}

Application::Application() {
    init();

    eventDispatcher.sink<KeyEvent>().connect<&Application::onKeyEvent>(*this);
    eventDispatcher.sink<FramebufferSizeEvent>().connect<&Application::onFramebufferSizeEvent>(*this);
}

void Application::run() {
    glfwSetTime(0);
    float lastTime = 0;

    while (!stopRequested) {
        float currentTime = (float)glfwGetTime();
        float dt = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.0f, 0.698f, 0.894f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (System* system : systems) {
            system->update(dt);
        }

        gui->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        stopRequested |= glfwWindowShouldClose(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
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

GLFWwindow* Application::getWindow() const {
    return window;
}

void Application::onKeyEvent(const KeyEvent& e) {
    if (e.action == GLFW_PRESS) {
        switch (e.key) {
        case GLFW_KEY_ESCAPE:
            // change gui visibility
            if (gui->visible) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                gui->visible = false;
                gamePaused = false;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                gui->visible = true;
                gamePaused = true;
            }
            break;
        }
    }
}

void Application::onFramebufferSizeEvent(const FramebufferSizeEvent& e) {
    gui->setScreenSize(e.width, e.height);
}

template<typename Event>
void Application::raiseEvent(const Event& event) {
    eventDispatcher.trigger<Event>(event);
}

template void Application::raiseEvent<FramebufferSizeEvent>(const FramebufferSizeEvent&);
template void Application::raiseEvent<KeyEvent>(const KeyEvent&);
template void Application::raiseEvent<MouseMoveEvent>(const MouseMoveEvent&);