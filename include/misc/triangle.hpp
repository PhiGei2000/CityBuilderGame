#pragma once
#include <glm/glm.hpp>

struct Ray;

struct Triangle {
    glm::vec3 point;
    glm::vec3 v1, v2;

    Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

    std::pair<bool, float> intersectionPoint(const Ray& ray) const;
};
