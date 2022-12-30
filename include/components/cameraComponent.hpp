#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct TransformationComponent;

struct CameraComponent : public Component<false> {
    glm::vec3 up, front, right;

    float width = 800.0f;
    float height = 600.0f;
    float fov = 60.0f;
    float near = 0.1f;
    float far = 200.0f;

    float yaw = 0.0f, pitch = 0.0f;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    void calculateMatrices(const TransformationComponent& transform);

    void calculateVectors();
};
