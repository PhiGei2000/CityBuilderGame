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
#include "roadTypes.hpp"

#include <functional>

#include <glm/glm.hpp>

enum class RoadTileTypes : unsigned char {
    NOT_CONNECTED,
    END,
    CURVE,
    T_CROSSING,
    CROSSING,
    STRAIGHT,
    CURVE_FULL,
    RAMP,
    UNDEFINED = 254U,
    EMPTY = 255U
};

inline RoadTileTypes operator++(RoadTileTypes& t, int) {
    if (t == RoadTileTypes::UNDEFINED || t == RoadTileTypes::EMPTY) {
        return t;
    }

    RoadTileTypes prevValue = t;
    t = static_cast<RoadTileTypes>(static_cast<unsigned char>(t) + 1);
    return prevValue;
}

constexpr std::string getRoadTileTypeName(RoadTileTypes type);

struct RoadTile {
    RoadTileTypes tileType = RoadTileTypes::EMPTY;
    int rotation = 0;
    RoadTypes roadType = RoadTypes::BASIC_ROADS;

    bool isRoadNode() const;

    bool empty() const;
    bool notEmpty() const;

    bool operator==(const RoadTile& other) const;
    bool operator!=(const RoadTile& other) const;
};

struct RoadRenderData {
    glm::vec3 position;
    glm::mat3 rotation;
};

using RoadTypeID = std::pair<RoadTypes, RoadTileTypes>;

std::string roadTypeID_toString(const RoadTypeID& id);

namespace std {
    template<>
    struct less<RoadTypeID> {
        constexpr bool operator()(const RoadTypeID& lhs, const RoadTypeID& rhs) const {
            return less<int>().operator()((static_cast<int>(lhs.first) << 8) + static_cast<int>(lhs.second), (static_cast<int>(rhs.first) << 8) + static_cast<int>(rhs.second));
        }
    };
} // namespace std
