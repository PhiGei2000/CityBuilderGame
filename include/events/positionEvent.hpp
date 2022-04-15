#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

struct PositionEvent {
    entt::entity entity;

    glm::vec2 position;
};
