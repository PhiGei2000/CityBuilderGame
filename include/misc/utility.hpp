/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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

    constexpr glm::ivec2 interpolate(const glm::ivec2& start, const glm::ivec2& end, int value) {
        return (end - start) * value + start;
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
