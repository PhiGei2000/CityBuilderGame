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
#include "misc/roads/roadTile.hpp"


std::string roadTileTypeNames[] = {
    "NOT_CONNECTED",
    "END",
    "CURVE",
    "T_CROSSING",
    "CROSSING",
    "STRAIGHT",
    "CURVE_FULL",
    "RAMP",
    "UNDEFINED",
    "EMPTY"};

constexpr std::string getRoadTileTypeName(RoadTileTypes type) {
    return roadTileTypeNames[static_cast<int>(type)];
}

bool RoadTile::isRoadNode() const {
    switch (tileType) {
        case RoadTileTypes::STRAIGHT:
        case RoadTileTypes::RAMP:
        case RoadTileTypes::UNDEFINED:
            return false;
        default:
            return true;
    }
}

bool RoadTile::empty() const {
    return tileType == RoadTileTypes::EMPTY;
}

bool RoadTile::notEmpty() const {
    return !empty();
}

std::string roadTypeID_toString(const RoadTypeID& id) {
    return getRoadTypeName(id.first) + "." + getRoadTileTypeName(id.second);
}
