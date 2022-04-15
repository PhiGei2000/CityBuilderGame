#pragma once

#include "misc/carPath.hpp"
#include "misc/direction.hpp"

#include <queue>

struct CarComponent {
    // path to drive
    std::queue<glm::ivec2> path;

    // current driving direction
    Direction drivingDirection = Direction::UNDEFINED;

    // next destination
    glm::vec2 destination;

    // car is driving
    bool driving = false;

    // car is in turn
    bool inTurn = false;
};
