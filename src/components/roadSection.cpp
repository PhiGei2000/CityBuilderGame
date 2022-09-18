#include "misc/roads/roadSection.hpp"

RoadSection::RoadSection(const glm::ivec2& position, const std::array<bool, 4>& connections)
    : position(position), connections(connections) {
}

RoadSection::RoadSection(const glm::ivec2& start, const glm::ivec2& end)
    : position(start) {
    sectionVector = end - start;

    if (sectionVector.x == 0) {
        connections = {false, true, false, true};
    }
    else {
        connections = {true, false, true, false};
    }
}

int RoadSection::getRotation() const {
    RoadType type = getType();

    switch (type) {
        case RoadType::EDGE:
            return connections[0] ? 1 : 0;
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

int RoadSection::getLength() const {
    return glm::abs(sectionVector.x) + glm::abs(sectionVector.y);
}

RoadType RoadSection::getType() const {
    if (getLength() != 0) {
        return RoadType::EDGE;
    }

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

bool RoadSection::containsPosition(const glm::ivec2& pos) const {
    if (getLength() == 0) {
        return position == pos;
    }

    const glm::ivec2& p = pos - position;
    const glm::ivec2& normal = glm::ivec2(sectionVector.y, sectionVector.x);

    if (p.x * normal.x + p.y * normal.y != 0)
        return false;

    int lambda = p.x * sectionVector.x + p.y * sectionVector.x;
    int pl = glm::abs(p.x) + glm::abs(p.y);
    int sl = getLength();

    return lambda >= 0 && pl <= sl;
}
