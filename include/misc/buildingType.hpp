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
    BUILDING_CATEGORY_COUNT,
    UNDEFINED
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

