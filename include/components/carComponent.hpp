#pragma once

#include "misc/carPath.hpp"

#include <queue>

struct CarComponent {
    // path to drive
    std::queue<CarPath> path;
};
