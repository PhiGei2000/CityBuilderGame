#pragma once
#include <iostream>

#include <glm/glm.hpp>

#include "configuration.hpp"

template<int size, typename T>
std::ostream& operator<<(std::ostream& os, const glm::vec<size, T>& vec);

namespace utility {

    template<typename T>
    constexpr bool inRange(T value, T minValue, T maxValue) {
        return minValue <= value && maxValue >= value;
    }

    constexpr glm::vec3 toWorldCoords(const glm::vec2& gridCoords) {
        return static_cast<float>(Configuration::gridSize) * glm::vec3{gridCoords.x, 0, gridCoords.y};
    }
    
} // namespace Utility