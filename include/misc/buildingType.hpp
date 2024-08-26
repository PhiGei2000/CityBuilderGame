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
#include <algorithm>
#include <string>
#include <map>
#include <stdexcept>
#include <format>

enum class BuildingCategory : unsigned int {
    INFRASTRUCTURE,
    RESIDENTIAL,
    INDUSTRIAL,
    BUILDING_CATEGORY_COUNT
};

inline const BuildingCategory getBuildingCategory(const std::string& str) {
    std::string lower_str = str;
    std::transform(str.begin(), str.end(), lower_str.begin(), [](unsigned char c) { return std::tolower(c); });

    if (lower_str == "infrastructure") {
        return BuildingCategory::INFRASTRUCTURE;
    }
    else if (lower_str == "residential") {
        return BuildingCategory::RESIDENTIAL;
    }
    else if (lower_str == "industrial") {
        return BuildingCategory::INDUSTRIAL;
    }

    throw std::invalid_argument(std::format("unknown BuildingCategory: \"{}\"", str));
}

inline const std::string to_string(const BuildingCategory category) {
    static const std::map<BuildingCategory, std::string> names = {
        std::make_pair(BuildingCategory::INFRASTRUCTURE, "infrastructure"),
        std::make_pair(BuildingCategory::RESIDENTIAL, "residual"),
        std::make_pair(BuildingCategory::INDUSTRIAL, "industrial"),
    };

    return names.at(category);
}

