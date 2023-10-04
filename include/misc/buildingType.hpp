#pragma once

enum class BuildingType : unsigned int {
    LIFT_TERRAIN,
    LOWER_TERRAIN,
    CLEAR,
    ROAD,
    PARKING_LOT,
};


#include <string>
#include <iostream>

inline std::ostream& operator<<(std::ostream& os, BuildingType type) {
    static std::string typeNames[] = {"LIFT_TERRAIN", "LOWER_TERRAIN", "DEFAULT", "ROAD", "PARKING_LOT"};

    return os << typeNames[(unsigned int)type];
}
