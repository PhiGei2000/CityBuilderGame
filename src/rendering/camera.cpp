#include "rendering/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace trafficSimulation {
    glm::mat4 Camera::getProjectionMatrix() const {
        return glm::ortho(0.0f, viewWidth, 0.0f, viewHeight, 0.1f, 100.0f);
    }

    glm::mat4 Camera::getViewMatrix() const {
        glm::mat4 view = glm::lookAt(glm::vec3(position.x, position.y, 1.0f),
                                     glm::vec3(position.x, position.y, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }
} // namespace trafficSimulation