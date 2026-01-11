#include "misc/roads/roadTile.hpp"

bool RoadTile::isRoadNode() const {
    switch (tileType) {
        case RoadTileTypes::STRAIGHT:
        case RoadTileTypes::RAMP:
        case RoadTileTypes::UNDEFINED:
        case RoadTileTypes::EMPTY:
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

glm::mat4 RoadTile::getTransform(const glm::vec3& pos) const {
    constexpr int sinValues[] = {0, 1, 0, -1};
    constexpr int cosValues[] = {1, 0, -1, 0};

    float cos = cosValues[rotation];
    float sin = sinValues[rotation];

    return glm::mat4(glm::vec4(cos, 0.0f, sin, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(-sin, 0.0f, cos, 0.0f), glm::vec4(pos, 1.0f));
}

bool RoadTile::operator==(const RoadTile& other) const {
    return tileType == other.tileType && rotation == other.rotation && roadType == other.roadType;
}

bool RoadTile::operator!=(const RoadTile& other) const {
    return !operator==(other);
}

std::string roadTypeID_toString(const RoadTypeID& id) {
    return id.first + "." + getRoadTileTypeName(id.second);
}
