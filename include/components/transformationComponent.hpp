#pragma once
#include "component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformationComponent : public Component {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    glm::mat4 transform;

    TransformationComponent(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

    void calculateTransform();

    void translate(const glm::vec3& translation);
    void setPosition(const glm::vec3& position);

    void rotate(const glm::vec3& axis, float angle);
    void setRotation(const glm::vec3& axis, float angle);
    void setRotation(const glm::vec3& eulerAngles);

    void addScale(const glm::vec3& scale);
    void setScale(const glm::vec3& scale);
};