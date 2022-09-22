#pragma once
#include "typedefs.hpp"

#include <glm/glm.hpp>
#include <map>

enum class Direction : byte {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    UNDEFINED
};

// component    | direction
// ------------------------
// positive x   | north
// positive z   | east
// negative x   | south
// negative z   | west

static std::map<Direction, glm::ivec2> DirectionVectors = {
    std::make_pair<Direction, glm::ivec2>(Direction::NORTH, glm::ivec2{1, 0}),
    std::make_pair<Direction, glm::ivec2>(Direction::EAST, glm::ivec2{0, 1}),
    std::make_pair<Direction, glm::ivec2>(Direction::SOUTH, glm::ivec2{-1, 0}),
    std::make_pair<Direction, glm::ivec2>(Direction::WEST, glm::ivec2{0, -1}),
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
