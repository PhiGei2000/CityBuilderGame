#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace trafficSimulation::components {
    struct TransformationComponent {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        glm::mat4 transform;

        void calculateTransform();

        void translate(const glm::vec3& translation);
        void setPosition(const glm::vec3& position);

        void rotate(const glm::vec3& axis, float angle);
        void setRotation(const glm::vec3& axis, float angle);
        void setRotation(const glm::vec3& eulerAngles);

        void addScale(const glm::vec3& scale);
        void setScale(const glm::vec3& scale);
    };
} // namespace trafficSimulation::components