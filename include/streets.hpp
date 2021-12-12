#pragma once
#include <iostream>

using byte = unsigned char;

namespace trafficSimulation {
    constexpr int connectionMask = 0b0011110;

    enum class StreetID : byte
    {
        SINGLE_STREET,
        STREET_END_BOTTOM,
        STREET_END_RIGHT,
        CURVE_TOP_LEFT,
        STREET_END_TOP,
        VERTICAL_STREET,
        CURVE_BOTTOM_LEFT,
        VERTICAL_STREET_LEFT,
        STREET_END_LEFT,
        CURVE_TOP_RIGHT,
        HORIZONTAL_STREET,
        HORIZONTAL_STREET_TOP,
        CURVE_BOTTOM_RIGHT,
        VERTICAL_STREET_RIGHT,
        HORIZONTAL_STREET_BOTTOM,
        CROSSING,
        NO_STREET
    };

    namespace Street {
        bool haveCommonConnections(StreetID s1, StreetID s2);
        StreetID merge(StreetID s1, StreetID s2);

        bool connectedTop(StreetID type);

        bool connectedLeft(StreetID type);

        bool connectedBottom(StreetID type);

        bool connectedRight(StreetID type);
    } // namespace Street

    std::ostream& operator<<(std::ostream& os, StreetID street);
} // namespace trafficSimulation