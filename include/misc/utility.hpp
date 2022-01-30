#pragma once
#include <iostream>

#include <glm/glm.hpp>

template<int size, typename T>
std::ostream& operator<<(std::ostream& os, const glm::vec<size, T>& vec);

namespace Utility {

    template<typename T>
    constexpr bool inRange(T value, T minValue, T maxValue) {
        return minValue <= value && maxValue >= value;
    }
} // namespace Utility