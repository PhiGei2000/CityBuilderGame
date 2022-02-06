#include "misc/carPath.hpp"

CarPath CarPath::leftTurn(const glm::vec2& start, float radius, float angle) {
    glm::vec2 end = radius * glm::vec2(1 - glm::cos(angle), glm::sin(angle)) + start;

    return CarPath{
        start,
        end,
        CarPathType::LEFT_TURN,
        radius,
    };
}

CarPath CarPath::rightTurn(const glm::vec2& start, float radius, float angle) {
    glm::vec2 end = radius * glm::vec2(glm::cos(angle) - 1, glm::sin(angle)) + start;

    return CarPath{
        start,
        end,
        CarPathType::RIGHT_TURN,
        radius,
    };
}

glm::vec3 CarPath::getAngularVelocity(float linearVelocity) const {
    if (radius == 0) {
        return glm::vec3(0);
    }

    return glm::vec3(
        0.0f, linearVelocity / radius, 0.0f
    );
}
