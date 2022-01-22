#pragma once
#include "typedefs.hpp"

#include <glm/glm.hpp>

enum class Direction : byte
{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    UNDEFINED
};

namespace misc {
    inline Direction getDirection(const glm::ivec2& vec) {
        // (vec.x != 0) xor (vec.y != 0)
        if ((vec.x == 0) != (vec.y == 0)) {
            if (vec.x != 0) {
                if (vec.x > 0) {
                    return Direction::EAST;
                }
                else {
                    return Direction::WEST;
                }
            }
            else if (vec.y != 0) {
                if (vec.y > 0) {
                    return Direction::NORTH;
                }
                else {
                    return Direction::SOUTH;
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
} // namespace misc
