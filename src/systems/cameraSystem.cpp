#include "systems/cameraSystem.hpp"

#include "components/components.hpp"

#include <GLFW/glfw3.h>

using namespace trafficSimulation::components;
using namespace trafficSimulation::events;

namespace trafficSimulation::systems {
    void CameraSystem::init() {
    }

    CameraSystem::CameraSystem(Application* app)
        : System(app) {
        cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();

        eventDispatcher.sink<FramebufferSizeEvent>()
            .connect<&CameraSystem::onFramebufferSize>(*this);
        eventDispatcher.sink<MouseMoveEvent>()
            .connect<&CameraSystem::onMouseMove>(*this);
        eventDispatcher.sink<KeyEvent>()
            .connect<&CameraSystem::onKey>(*this);
    }

    void CameraSystem::update(int dt) {
        CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
        TransformationComponent& transform = registry.get<TransformationComponent>(cameraEntity);

        glm::vec3 xzCameraFront = glm::vec3(camera.front.x, 0.0f, camera.front.z);
        glm::vec3 xzCameraRight = glm::vec3(camera.right.x, 0.0f, camera.right.z);

        const static float cameraSpeed = 0.1f;
        GLFWwindow* window = app->getWindow();

        glm::vec3 cameraMoveDirection;
        if (glfwGetKey(window, GLFW_KEY_W)) {
            cameraMoveDirection += xzCameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            cameraMoveDirection -= xzCameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            cameraMoveDirection += xzCameraRight;
        }
        if (glfwGetKey(window, GLFW_KEY_A)) {
            cameraMoveDirection -= xzCameraRight;
        }
        
        transform.position += cameraSpeed * glm::normalize(cameraMoveDirection);
    }

    void CameraSystem::onMouseMove(const MouseMoveEvent& e) {
        CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
        TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

        float dx = e.lastX - e.x;
        float dy = e.y - e.lastY;

        const float sensitivity = 0.1f;
        dx *= sensitivity;
        dy *= sensitivity;

        yaw += dx;
        pitch = glm::clamp(pitch + dy, -89.0f, 89.0f);

        cameraTransform.setRotation(glm::vec3(pitch, yaw, 0.0f));
        camera.calculateMatrices(cameraTransform);
    }

    void CameraSystem::onKey(const KeyEvent& e) {
    }

    void CameraSystem::onFramebufferSize(const FramebufferSizeEvent& e) {
        CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
        const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);

        camera.viewWidth = e.width;
        camera.viewHeight = e.height;

        camera.calculateMatrices(cameraTransform);
    }
} // namespace trafficSimulation::systems