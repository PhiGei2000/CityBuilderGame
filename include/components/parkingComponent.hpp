#pragma once
#include "component.hpp"
#include "misc/direction.hpp"

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
    bool occupied;
    /// @brief The direction of the parking spot
    Direction direction;
    /// @brief The entity occupying the parking spot
    entt::entity entity = entt::null;

    inline ParkingSpot(const std::string& id, const glm::vec3& position, Direction direction, bool occupied = false)
        : id(id), position(position), direction(direction), occupied(occupied) {
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
