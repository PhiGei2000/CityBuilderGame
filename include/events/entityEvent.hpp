#pragma once
#include "event.hpp"

#include <entt/entt.hpp>

struct EntityEvent : public Event {
    entt::entity entity;

    inline EntityEvent(entt::entity entity)
        : entity(entity) {
    }
};