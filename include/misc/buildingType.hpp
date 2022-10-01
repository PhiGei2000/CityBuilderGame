#pragma once

enum class BuildingType : unsigned int {
    CLEAR = 1,
    ROAD = 2,
    PARKING_LOT = 3
};


#include <string>
#include <iostream>

inline std::ostream& operator<<(std::ostream& os, BuildingType type) {
    static std::string typeNames[] = {"DEFAULT", "ROAD", "PARKING_LOT"};

    return os << typeNames[(unsigned int)type - 1];
}
