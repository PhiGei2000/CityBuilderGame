#pragma once

#include <vector>

#include <glm/glm.hpp>

enum class CarPathType
{
    STRAIGHT,
    LEFT_TURN,
    RIGHT_TURN
};

struct CarPath {
    glm::vec2 start;
    glm::vec2 end;

    CarPathType type;
    float radius = 0;

    static CarPath leftTurn(const glm::vec2& start, float radius, float angle);

    static CarPath rightTurn(const glm::vec2& start, float radius, float angle);

    glm::vec3 getAngularVelocity(float linearVelocity) const;
};
