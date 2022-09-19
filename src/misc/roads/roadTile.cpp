#include "misc/roads/roadTile.hpp"

RoadTile::RoadTile()
    : RoadTile(undefinedPosition) {
}

RoadTile::RoadTile(const glm::ivec2& position, const std::array<bool, 4>& connections)
    : position(position), connections(connections) {
}

int RoadTile::getRotation() const {
    RoadType type = getType();

    switch (type) {
        case RoadType::EDGE:
            return connections[0] ? 0 : 1;
        case RoadType::END: {
            int i = 0;
            while (!connections[i])
                i++;

            return i;
        }
        case RoadType::CURVE:
        case RoadType::CURVE_FULL: {
            int i = 0;
            while (!connections[i])
                i++;

            if (i != 0) {
                return i;
            }

            return connections[3] ? 3 : 0;
        }
        case RoadType::T_CROSSING: {
            int i = 0;
            while (connections[i])
                i++;

            return i - 3;
        }
        default:
            return 0;
    }
}

RoadType RoadTile::getType() const {
    int connectionsCount = (connections[0] ? 1 : 0) + (connections[1] ? 1 : 0) + (connections[2] ? 1 : 0) + (connections[3] ? 1 : 0);

    switch (connectionsCount) {
        case 0:
            return RoadType::NOT_CONNECTED;
        case 1:
            return RoadType::END;
        case 2:
            if (connections[0] == connections[2]) {
                return RoadType::EDGE;
            }
            else {
                return RoadType::CURVE;
            }
        case 3:
            return RoadType::T_CROSSING;
        case 4:
            return RoadType::CROSSING;
        default:
            return RoadType::UNDEFINED;
    }
}

void RoadTile::addConnections(const Connections& connections) {
    this->connections[0] |= connections[0];
    this->connections[1] |= connections[1];
    this->connections[2] |= connections[2];
    this->connections[3] |= connections[3];
}
