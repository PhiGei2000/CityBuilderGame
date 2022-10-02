#pragma once
#include "component.hpp"

#include <vector>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

/// @brief Describes a parking spot and the entity occupying it.
struct ParkingSpot {
    /// @brief The position of the parking spot
    glm::vec3 position;
    /// @brief True if the parking spot is occupied by an entity, otherwise false
    bool occupied = false;
    /// @brief The entity occupying the parking spot
    entt::entity entity = entt::entity();

    inline ParkingSpot(const glm::vec3& position)
        : position(position) {
    }
};

struct ParkingComponent : public Component {
    std::vector<ParkingSpot> parkingSpots;

    inline ParkingComponent(const std::vector<ParkingSpot>& parkingSpots)
        : parkingSpots(parkingSpots) {
    }
};