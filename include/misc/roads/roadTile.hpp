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
    // CURVE_FULL,
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

constexpr std::string getRoadTileTypeName(RoadTileTypes type) {
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

    switch (type) {
        case RoadTileTypes::UNDEFINED:
            return "Undefined";
        case RoadTileTypes::EMPTY:
            return "Empty";
        default:
            return roadTileTypeNames[static_cast<int>(type)];
    }
}

struct RoadTile {
    RoadTileTypes tileType = RoadTileTypes::EMPTY;
    int rotation = 0;
    std::string roadType;

    bool isRoadNode() const;

    bool empty() const;
    bool notEmpty() const;

    glm::mat4 getTransform(const glm::vec3& offset) const;

    bool operator==(const RoadTile& other) const;
    bool operator!=(const RoadTile& other) const;
};

struct RoadRenderData {
    glm::vec3 position;
    glm::mat3 rotation;
};

using RoadTypeID = std::pair<std::string, RoadTileTypes>;

std::string roadTypeID_toString(const RoadTypeID& id);

namespace std {
    template<>
    struct less<RoadTypeID> {
        constexpr bool operator()(const RoadTypeID& lhs, const RoadTypeID& rhs) const {
            return std::less<std::string>()(lhs.first, rhs.first) || (std::equal_to<std::string>()(lhs.first, rhs.first) && lhs.second < rhs.second);

            // return less<int>().operator()((static_cast<int>(lhs.first) << 8) + static_cast<int>(lhs.second), (static_cast<int>(rhs.first) << 8) + static_cast<int>(rhs.second));
        }
    };
} // namespace std
