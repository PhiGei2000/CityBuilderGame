#pragma once
#include "event.hpp"

#include <entt/entt.hpp>

template<bool ignoreState>
struct EntityEvent : public Event<ignoreState> {
    entt::entity entity;

    inline EntityEvent(entt::entity entity)
        : entity(entity) {
    }
};
