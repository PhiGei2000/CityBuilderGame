#pragma once

#include "misc/streetTypes.hpp"

#include <entt/entt.hpp>

#include <vector>

struct StreetComponent {
    StreetType type;

    std::vector<entt::entity> nextCrossings;
};
