#pragma once
#include "component.hpp"

#include <queue>

#include <glm/glm.hpp>

struct CarComponent : public AssignableComponent {    
    bool driving = false;
    
    glm::vec3 lastPathPosition;
    std::queue<glm::vec3> currentPath;

    inline CarComponent() {
    }    

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<CarComponent>(entity);
    }
};