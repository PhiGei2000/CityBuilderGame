#pragma once
#include "event.hpp"

#include "misc/buildingType.hpp"

#include <glm/glm.hpp>

enum class BuildAction {
    DEFAULT,
    BEGIN,
    END,
    ENTITY_CREATED,
    SELECT,
    PREVIEW
};

enum class BuildShape {
    POINT,
    LINE,
    AREA
};

struct BuildEvent : public Event {
    glm::ivec2 gridPosition;
    BuildingType type;

    BuildAction action;
    glm::ivec2 startPosition = glm::ivec2(-1);
    BuildShape shape = BuildShape::LINE;
    bool xFirst = false;

    inline BuildEvent(const glm::ivec2& gridPosition, BuildingType type, BuildAction action, const glm::ivec2& startPosition = glm::ivec2(-1), BuildShape shape = BuildShape::LINE, bool xFirst = false)
        : gridPosition(gridPosition), type(type), action(action), startPosition(startPosition), shape(shape), xFirst(xFirst) {
    }
};
