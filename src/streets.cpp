#include "streets.hpp"

namespace trafficSimulation::Street {
    bool haveCommonConnections(StreetID s1, StreetID s2) {
        byte connections1 = (byte)s1 & connectionMask;
        byte connections2 = (byte)s2 & connectionMask;

        return connections1 & connections2;
    }

    StreetID merge(StreetID s1, StreetID s2) {
        if (s1 == StreetID::NO_STREET) {
            return s2;
        }
        else if (s2 == StreetID::NO_STREET) {
            return s1;
        }
        else {
            return (StreetID)((byte)s1 | (byte)s2);
        }
    }

    bool connectedTop(StreetID type) {
        return (byte)type & 1;
    }

    bool connectedLeft(StreetID type) {
        return (byte)type & 2;
    }

    bool connectedBottom(StreetID type) {
        return (byte)type & 4;
    }

    bool connectedRight(StreetID type) {
        return (byte)type & 8;
    }

} // namespace trafficSimulation::Street

namespace trafficSimulation {

    std::ostream& operator<<(std::ostream& os, StreetID street) {
        switch (street) {
        case StreetID::SINGLE_STREET:
            os << "single street";
            break;
        case StreetID::STREET_END_BOTTOM:
            os << "street end bottom";
            break;
        case StreetID::STREET_END_RIGHT:
            os << "street end right";
            break;
        case StreetID::CURVE_TOP_LEFT:
            os << "curve top left";
            break;
        case StreetID::STREET_END_TOP:
            os << "street end top";
            break;
        case StreetID::VERTICAL_STREET:
            os << "vertical street";
            break;
        case StreetID::CURVE_BOTTOM_LEFT:
            os << "curve bottom left";
            break;
        case StreetID::VERTICAL_STREET_LEFT:
            os << "vertical street left";
            break;
        case StreetID::STREET_END_LEFT:
            os << "street end left";
            break;
        case StreetID::CURVE_TOP_RIGHT:
            os << "curve top right";
            break;
        case StreetID::HORIZONTAL_STREET:
            os << "horizontal street";
            break;
        case StreetID::HORIZONTAL_STREET_TOP:
            os << "horizontal street top";
            break;
        case StreetID::CURVE_BOTTOM_RIGHT:
            os << "curve bottom right";
            break;
        case StreetID::VERTICAL_STREET_RIGHT:
            os << "vertical street right";
            break;
        case StreetID::HORIZONTAL_STREET_BOTTOM:
            os << "horizontal street bottom";
            break;
        case StreetID::CROSSING:
            os << "crossing";
            break;
        case StreetID::NO_STREET:
            os << "No street";
            break;
        }

        return os;
    }
} // namespace trafficSimulation
