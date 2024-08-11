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
#include "typedefs.hpp"

#include <glm/glm.hpp>
#include <map>

enum class Direction : byte {
    /// @brief Positive x
    NORTH,
    /// @brief Positive z
    EAST,
    /// @brief Negative x
    SOUTH,
    /// @brief Negative z
    WEST,
    UNDEFINED
};

// component    | direction
// ------------------------
// positive x   | north
// positive z   | east
// negative x   | south
// negative z   | west

template<typename T = glm::vec2>
static std::map<Direction, T> DirectionVectors = {
    std::make_pair<Direction, T>(Direction::NORTH, T{1.0f, 0.0f}),
    std::make_pair<Direction, T>(Direction::EAST, T{0.0f, 1.0f}),
    std::make_pair<Direction, T>(Direction::SOUTH, T{-1.0f, 0.0f}),
    std::make_pair<Direction, T>(Direction::WEST, T{0.0f, -1.0f}),
};

namespace utility {
    inline Direction getDirection(const glm::vec2& vec) {
        // (vec.x != 0) xor (vec.y != 0)
        if ((vec.x == 0) != (vec.y == 0)) {
            if (vec.x != 0) {
                if (vec.x > 0) {
                    return Direction::NORTH;
                }
                else {
                    return Direction::SOUTH;
                }
            }
            else if (vec.y != 0) {
                if (vec.y > 0) {
                    return Direction::EAST;
                }
                else {
                    return Direction::WEST;
                }
            }
        }

        return Direction::UNDEFINED;
    }

    inline Direction getInverse(Direction dir) {
        return dir == Direction::UNDEFINED
                   ? dir
                   : static_cast<Direction>((static_cast<byte>(dir) + 2) % 4);
    }

    inline bool isNorthSouth(Direction dir) {
        return dir == Direction::NORTH || dir == Direction::SOUTH;
    }

    inline bool isEastWest(Direction dir) {
        return dir == Direction::EAST || dir == Direction::WEST;
    }
} // namespace utility

inline Direction operator-(const Direction& other) {
    return utility::getInverse(other);
}

inline Direction operator++(Direction& dir, int i) {
    Direction tmp = dir;
    int newDir = (int)dir + 1;

    dir = newDir < 5 ? (Direction)newDir : Direction::UNDEFINED;

    return tmp;
}
