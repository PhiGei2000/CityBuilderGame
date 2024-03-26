/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
