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
