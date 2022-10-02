#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct BuildingComponent : public Component {
    glm::ivec2 gridPosition;
    glm::ivec2 size = glm::ivec2(1);    

    inline BuildingComponent(const glm::ivec2& gridPosition, const glm::ivec2& size = glm::ivec2(1))
        : gridPosition(gridPosition), size(size) {
    }
};
