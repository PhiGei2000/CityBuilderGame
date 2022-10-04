#pragma once
#include <entt/entt.hpp>

template<bool assignable>
struct Component {
};

template<>
struct Component<true> {
    inline virtual void assignToEntity(const entt::entity entity, entt::registry& registry) const = 0;
};

using AssignableComponent = Component<true>;

#include <concepts>

template<typename T>
concept ComponentType = (std::derived_from<T, Component<false>> || std::derived_from<T, Component<true>>)&&std::copy_constructible<T>;

template<typename T>
concept AssignableComponentType = std::derived_from<T, Component<true>> && std::copy_constructible<T>;
