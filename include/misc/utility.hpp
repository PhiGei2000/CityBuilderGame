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

namespace std {
    template<typename T, glm::qualifier Q>
    std::string to_string(const glm::vec<3, T, Q>& vec) {
        return "x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + " z: " + std::to_string(vec.z);
    }
} // namespace std

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

    inline glm::vec3 cartesianToSpherical(const glm::vec3& coords) {
        float r = glm::length(coords);

        float theta = glm::acos(coords.y / r);
        float phi = (coords.x == 0 && coords.z == 0) ? 0 : glm::sign(coords.x) * glm::acos(coords.x / glm::sqrt(coords.x * coords.x + coords.z * coords.z));
        return glm::vec3(r, theta, phi);
    }

    inline glm::vec3 cartesianToSpherical(float x, float y, float z) {
        return cartesianToSpherical(glm::vec3(x, y, z));
    }

    inline glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
        return r * glm::vec3(
                       glm::sin(theta) * glm::cos(phi),
                       glm::cos(theta),
                       glm::sin(theta) * glm::sin(phi));
    }

    inline glm::vec3 sphericalToCartesian(const glm::vec3& coords) {
        return sphericalToCartesian(coords.x, coords.y, coords.z);
    }
} // namespace utility

namespace glm {
    template<>
    GLM_FUNC_QUALIFIER int length(const vec<2, int, packed_highp>& v) {
        return abs(v.x) + abs(v.y);
    }

    template<>
    GLM_FUNC_QUALIFIER vec<2, int, packed_highp> normalize(const vec<2, int, packed_highp>& v) {
        return vec<2, int, packed_highp>(
            v.x == 0 ? 0 : v.x / glm::abs(v.x),
            v.y == 0 ? 0 : v.y / glm::abs(v.y));
    }

    template<>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR int dot(const vec<2, int, packed_highp>& x, const vec<2, int, packed_highp>& y) {
        return x.x * y.x + x.y * y.x;
    }
} // namespace glm