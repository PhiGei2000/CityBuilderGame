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
#include <glm/glm.hpp>
#include <functional>

template<>
inline constexpr bool std::less<glm::ivec2>::operator()(const glm::ivec2& x, const glm::ivec2& y) const {
    if (x.x != y.x) {
        return x.y < y.y;
    }

    return x.x < x.y;

}

template<>
inline constexpr bool std::less<std::pair<glm::ivec2, glm::ivec2>>::operator()(const std::pair<glm::ivec2, glm::ivec2>& x, const std::pair<glm::ivec2, glm::ivec2>& y) const {
    std::less<glm::ivec2> nodeComparator;

    if (x.first == y.first) {
        return nodeComparator.operator()(x.second, y.second);
    }

    return nodeComparator.operator()(x.first, y.first);
}