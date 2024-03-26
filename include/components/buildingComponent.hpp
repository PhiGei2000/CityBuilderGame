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
#include "misc/buildingType.hpp"

#include <glm/glm.hpp>

struct BuildingComponent : public AssignableComponent {
    BuildingType type = BuildingType::NONE;
    glm::ivec2 gridPosition;
    int rotation = 0;
    glm::ivec2 size = glm::ivec2(0);
    bool preview = false;

    inline BuildingComponent(BuildingType type, const glm::ivec2& gridPosition, int rotation = 0, const glm::ivec2& size = glm::ivec2(0), bool preview = false)
        : type(type), gridPosition(gridPosition), rotation(rotation), size(size), preview(preview) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<BuildingComponent>(entity, gridPosition, rotation, size, preview);
    }
};
