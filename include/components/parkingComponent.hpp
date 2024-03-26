/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "component.hpp"

#include <vector>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

/// @brief Describes a parking spot and the entity occupying it.
struct ParkingSpot {
    /// @brief An id to identify the parking spot
    const std::string id;
    /// @brief The position of the parking spot
    glm::vec3 position;
    /// @brief True if the parking spot is occupied by an entity, otherwise false
    bool occupied = false;
    /// @brief The entity occupying the parking spot
    entt::entity entity = entt::null;

    inline ParkingSpot(const std::string& id, const glm::vec3& position)
        : id(id), position(position) {
    }
};

struct ParkingComponent : public AssignableComponent {
    std::vector<ParkingSpot> parkingSpots;

    inline ParkingComponent(const std::vector<ParkingSpot>& parkingSpots)
        : parkingSpots(parkingSpots) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<ParkingComponent>(entity, parkingSpots);
    }
};
