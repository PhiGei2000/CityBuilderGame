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
#include "entityEvent.hpp"

#include "misc/buildingType.hpp"
#include "misc/configuration.hpp"
#include "misc/utility.hpp"

#include <vector>

#include <glm/glm.hpp>

enum class BuildAction : unsigned int {
    END,
    SELECT
};

enum class BuildShape {
    POINT,
    LINE,
    AREA
};

/// @brief Holds data about building processes
struct BuildEvent : public EntityEvent {
    /// @brief The position where the building should be created
    std::vector<glm::ivec2> positions;

    /// @brief The type of the building
    BuildingType type;

    /// @brief The action to perform
    BuildAction action;

    /// @brief The shape of the building.
    BuildShape shape = BuildShape::LINE;

    /// @brief True if the build can be builded on the specified position
    bool valid = true;

    inline BuildEvent(entt::entity entity, const std::vector<glm::ivec2>& positions, BuildingType type, BuildAction action = BuildAction::END, BuildShape shape = BuildShape::LINE, bool valid = true)
        : EntityEvent(entity), positions(positions), type(type), action(action), shape(shape) {
    }

    inline bool insideArea(const glm::vec2& pos) const {
        const glm::ivec2 gridPosition = glm::floor(1 / static_cast<float>(Configuration::cellSize) * pos);

        switch (shape) {
            case BuildShape::POINT:
                return positions[0] == gridPosition;
            case BuildShape::AREA: {
                const glm::ivec2 min = glm::min(positions[0], positions[1]);
                const glm::ivec2 max = glm::max(positions[0], positions[1]);

                return utility::inRange(gridPosition.x, min.x, max.x) && utility::inRange(gridPosition.y, min.y, max.y);
            };
            case BuildShape::LINE: {
                int segementsCount = positions.size() - 1;

                for (int i = 0; i < segementsCount; i++) {
                    const glm::ivec2 min = glm::min(positions[i], positions[i + 1]);
                    const glm::ivec2 max = glm::max(positions[i], positions[i + 1]);

                    if (utility::inRange(gridPosition.x, min.x, max.x) && utility::inRange(gridPosition.y, min.y, max.y)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }
};
