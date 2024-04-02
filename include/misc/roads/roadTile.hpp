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

#include <glm/glm.hpp>

enum class RoadTileType : unsigned char
{
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

struct RoadTile {
    RoadTileType tileType = RoadTileType::EMPTY;
    int rotation = 0;
    RoadTypes roadType = RoadTypes::BASIC_STREETS;

    bool isRoadNode() const;

    bool empty() const;
    bool notEmpty() const;
};

struct RoadRenderData {
    glm::vec2 position;
    RoadTile roadTile;
};
