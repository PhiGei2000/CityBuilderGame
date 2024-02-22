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
