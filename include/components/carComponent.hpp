#pragma once
#include "component.hpp"

struct CarComponent : public AssignableComponent {
    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<CarComponent>(entity);
    }
};