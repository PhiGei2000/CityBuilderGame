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

struct Object {
  protected:
    std::map<std::type_index, std::shared_ptr<AssignableComponent>> components;

  public:
    std::string name;

    template<AssignableComponentType TComponent>
    void addComponent(const TComponent& component);

    entt::entity create(entt::registry& registry) const;
};

using ObjectPtr = ResourcePtr<Object>;
