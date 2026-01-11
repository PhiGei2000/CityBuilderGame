#pragma once
#include "component.hpp"
#include "misc/roads/path.hpp"

#include <glm/glm.hpp>

struct CarComponent : public AssignableComponent {
    bool driving = false;

    float positionOnPath;
    CarPath currentPath;

    inline CarComponent() {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<CarComponent>(entity);
    }
};
