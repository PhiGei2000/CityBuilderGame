#include "misc/triangle.hpp"

#include "misc/ray.hpp"
#include "misc/utility.hpp"

Triangle::Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    point = p1;
    v1 = p2 - p1;
    v2 = p3 - p1;
}

std::pair<bool, float> Triangle::intersectionPoint(const Ray& ray) const {
    glm::mat3 matrix = glm::mat3(-ray.direction, v1, v2);

    const glm::vec3& coefficients = glm::inverse(matrix) * (ray.start - point);

    bool inTriangle = utility::inRange(coefficients.y, 0.0f, 1.0f) && utility::inRange(coefficients.z, 0.0f, 1.0f - coefficients.y);

    return std::make_pair(inTriangle, coefficients.x);
}