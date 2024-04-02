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

enum class BuildingType : unsigned int {
    NONE,
    CLEAR,
    ROAD,
    PARKING_LOT,
};

#include <iostream>
#include <string>


inline std::ostream& operator<<(std::ostream& os, BuildingType type) {
    static std::string typeNames[] = {"NONE", "DEFAULT", "ROAD", "PARKING_LOT"};

    return os << typeNames[(unsigned int)type];
}

constexpr std::string getBuildingName(BuildingType type) {
    switch (type) {
        case BuildingType::PARKING_LOT:
            return "parking_lot";
        case BuildingType::ROAD:
            return "road";
    }

    return "";
}
