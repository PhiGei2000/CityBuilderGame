#pragma once
#include <entt/entt.hpp>

struct Component {    
};

#include <concepts>

template<typename T>
concept ComponentType = std::derived_from<T, Component> && std::copy_constructible<T>;
