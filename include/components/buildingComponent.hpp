#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct BuildingComponent : public AssignableComponent {
    /// @brief Type of the building
    std::string buildingID;
    /// @brief Coordinates of the south west corner in normalized world grid coordinates
    glm::ivec2 gridPosition;
    /// @brief Rotation
    int rotation = 0;
    /// @brief Size in x and y direction in cells
    glm::vec2 size = glm::vec2(0.0f);
    /// @brief If `true` the preview version will be rendered
    bool preview = false;

    inline BuildingComponent(std::string buildingID, const glm::ivec2& gridPosition, int rotation = 0, const glm::ivec2& size = glm::ivec2(0), bool preview = false)
        : buildingID(buildingID), gridPosition(gridPosition), rotation(rotation), size(size), preview(preview) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<BuildingComponent>(entity, buildingID, gridPosition, rotation, size, preview);
    }
};
