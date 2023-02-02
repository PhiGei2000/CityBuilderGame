#pragma once
#include "event.hpp"

#include "misc/buildingType.hpp"
#include "misc/configuration.hpp"
#include "misc/utility.hpp"

#include <vector>

#include <glm/glm.hpp>

enum class BuildAction : unsigned int {
    DEFAULT,
    BEGIN,
    END,
    ENTITY_CREATED,
    SELECT,
    PREVIEW,
    END_PREVIEW
};

enum class BuildShape {
    POINT,
    LINE,
    AREA
};

/// @brief Holds data about building processes
struct BuildEvent : public Event {
    /// @brief The position where the building should be created
    std::vector<glm::ivec2> positions;

    /// @brief The type of the building
    BuildingType type;

    /// @brief The action to perform
    BuildAction action;

    /// @brief The shape of the building.
    BuildShape shape = BuildShape::LINE;

    inline BuildEvent(const std::vector<glm::ivec2>& positions, BuildingType type, BuildAction action, BuildShape shape = BuildShape::LINE)
        : positions(positions), type(type), action(action), shape(shape) {
    }

    inline bool insideArea(const glm::vec2& pos) const {
        const glm::ivec2 gridPosition = glm::floor(1 / static_cast<float>(Configuration::gridSize) * pos);

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
