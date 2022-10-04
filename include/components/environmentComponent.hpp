#pragma once
#include "component.hpp"

struct EnvironmentComponent : public AssignableComponent {   
    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<EnvironmentComponent>(entity);
    }     
};
