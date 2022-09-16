#pragma once
#include <iostream>

#include <glm/glm.hpp>

#include "../misc/buildingType.hpp"
#include "configuration.hpp"

template<typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, const glm::vec<2, T, Q>& vec);
template<typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, const glm::vec<3, T, Q>& vec);
template<typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, const glm::vec<4, T, Q>& vec);

std::ostream& operator<<(std::ostream& os, BuildingType type);

namespace utility {

    template<typename T>
    constexpr bool inRange(T value, T minValue, T maxValue) {
        return minValue <= value && maxValue >= value;
    }

    constexpr glm::vec3 toWorldCoords(const glm::vec2& gridCoords) {
        return static_cast<float>(Configuration::gridSize) * glm::vec3{gridCoords.x, 0, gridCoords.y};
    }

    static glm::vec2 toPlaneCoords(const glm::vec2& gridCoords) {
        return static_cast<float>(Configuration::gridSize) * glm::floor(gridCoords);
    }

    static glm::ivec2 toGridCoords(const glm::vec2& position) {
        return glm::floor(1 / static_cast<float>(Configuration::gridSize) * position);
    }

    static glm::ivec2 toGridCoords(const glm::vec3& position) {
        return toGridCoords(glm::vec2{position.x, position.z});
    }

    constexpr glm::ivec2 interpolate(const glm::ivec2& start, const glm::ivec2& end, int value) {
        return (end - start) * value + start;
    }

} // namespace utility

namespace glm {
    template<>
    GLM_FUNC_QUALIFIER vec<2, int, packed_highp> normalize(const vec<2, int, packed_highp>& v) {
        return vec<2, int, packed_highp>(
            v.x == 0 ? 0 : v.x / v.x,
            v.y == 0 ? 0 : v.y / v.y);
    }
} // namespace glm