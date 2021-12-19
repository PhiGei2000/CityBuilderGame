#include "systems/cameraSystem.hpp"

#include "components/components.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

void CameraSystem::init() {
    entt::entity cameraEntity = registry.create();

    registry.emplace<CameraComponent>(cameraEntity);
    registry.emplace<TransformationComponent>(cameraEntity, glm::vec3(0.0f, 1.0f, 0.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
}

CameraSystem::CameraSystem(Application* app)
    : System(app) {

    init();
    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();

    eventDispatcher.sink<FramebufferSizeEvent>()
        .connect<&CameraSystem::onFramebufferSize>(*this);
    eventDispatcher.sink<MouseMoveEvent>()
        .connect<&CameraSystem::onMouseMove>(*this);
}

void CameraSystem::update(int dt) {
    if (app->gamePaused) {
        return;
    }

    CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    TransformationComponent& transform = registry.get<TransformationComponent>(cameraEntity);

    glm::vec3 xzCameraFront = glm::vec3(camera.front.x, 0.0f, camera.front.z);
    glm::vec3 xzCameraRight = glm::vec3(camera.right.x, 0.0f, camera.right.z);

    const static float cameraSpeed = 0.01f;
    GLFWwindow* window = app->getWindow();

    glm::vec3 cameraMoveDirection = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraMoveDirection += xzCameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraMoveDirection -= xzCameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraMoveDirection += xzCameraRight;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraMoveDirection -= xzCameraRight;
    }

    if (cameraMoveDirection.x != 0 && cameraMoveDirection.z != 0) {
        transform.position += cameraSpeed * glm::normalize(cameraMoveDirection);
        camera.calculateMatrices(transform);
    }
}

void CameraSystem::onMouseMove(const MouseMoveEvent& e) {
    if (app->gamePaused) {
        return;
    }

    CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

    float dx = e.x - e.lastX;
    float dy = e.lastY - e.y;

    const float sensitivity = 0.1f;
    dx *= sensitivity;
    dy *= sensitivity;

    camera.yaw += dx;
    camera.pitch = glm::clamp(camera.pitch + dy, -89.0f, 89.0f);

    camera.calculateMatrices(cameraTransform);    
}

void CameraSystem::onFramebufferSize(const FramebufferSizeEvent& e) {
    CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

    camera.width = e.width;
    camera.height = e.height;

    camera.calculateMatrices(cameraTransform);
}