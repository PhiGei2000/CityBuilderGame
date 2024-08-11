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
#include <memory>
#include <string>

enum class RoadTypes : unsigned char {
    BASIC_ROADS,
    UNDEFINED
};

inline RoadTypes operator++(RoadTypes& t, int) {
    if (t == RoadTypes::UNDEFINED) {
        return RoadTypes::UNDEFINED;
    }

    RoadTypes prevValue = t;
    t = static_cast<RoadTypes>(static_cast<unsigned char>(t) + 1);
    return prevValue;
}

inline constexpr std::string getRoadTypeName(RoadTypes type) {
    std::string roadTypeNames[] = {
        "BASIC_ROADS",
    };

    return roadTypeNames[static_cast<int>(type)];
}

struct RoadPack;
class ResourceManager;

std::shared_ptr<RoadPack> getRoadPack(const ResourceManager& resourceManager, RoadTypes type);
