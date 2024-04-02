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
#include "components/component.hpp"
#include "misc/typedefs.hpp"

#include <string>
#include <typeindex>
#include <vector>

/// @brief This structure is a template for an entity that holds various components.
struct Object {
  protected:
    std::map<std::type_index, std::shared_ptr<AssignableComponent>> components;

  public:
    /// @brief The name of the object
    std::string name;

    /// @brief Adds a template component to the object. The component type must be an assignable component.
    /// @tparam TComponent The type of the component
    /// @param component The component
    template<AssignableComponentType TComponent>
    inline void addComponent(const TComponent& component) {
        components[typeid(TComponent)] = std::make_shared<TComponent>(component);
    }

    /// @brief Creates an entity in the given registry and assings the associated components to this entity.
    /// @param registry The registry in which the component will be created
    /// @return The created entity
    inline entt::entity create(entt::registry& registry) const {
        const entt::entity entity = registry.create();

        for (const auto& [type, component] : components) {
            component->assignToEntity(entity, registry);
        }

        return entity;
    }
};

/// @brief A resource pointer to an object
using ObjectPtr = ResourcePtr<Object>;
