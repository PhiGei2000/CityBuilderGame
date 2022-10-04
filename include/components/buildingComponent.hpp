#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct BuildingComponent : public AssignableComponent {
    glm::ivec2 gridPosition;
    glm::ivec2 size = glm::ivec2(1);    

    inline BuildingComponent(const glm::ivec2& gridPosition, const glm::ivec2& size = glm::ivec2(1))
        : gridPosition(gridPosition), size(size) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<BuildingComponent>(entity, gridPosition, size);
    }
};
