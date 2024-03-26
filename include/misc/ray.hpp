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
#include <vector>

#include <glm/glm.hpp>


struct Ray {
    /// @brief Start of the ray
    glm::vec3 start;
    /// @brief Direction of the ray
    glm::vec3 direction;

    /// @brief Creates a ray from a start point and a direction
    /// @param start Start point of the ray
    /// @param direction Direction of the ray
    Ray(const glm::vec3& start, const glm::vec3& direction);

    /// @brief Creates a ray from a start point and a second point of the ray
    /// @param start The start point
    /// @param p Second point on the ray
    /// @return A ray with start point `start` going through `p`
    static Ray fromPoints(const glm::vec3& start, const glm::vec3& p);

    /// @brief Determines the cells which are crossed by the ray and returns the intersection points where the ray enters the cells
    /// @param maxLength Maximum distance in world coordinate units
    /// @return Pairs of cells (in normalized world grid coordinates) and intersection points (in world coordinates)
    std::vector<std::pair<glm::ivec2, glm::vec3>> getCellIntersections(float maxLength) const;
};
