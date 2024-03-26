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
#include "component.hpp"

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

struct CarPath {
    std::vector<glm::vec3> pathIn;
    std::vector<glm::vec3> pathOut;
};

struct CarPathComponent : public AssignableComponent {
    std::unordered_map<std::string, CarPath> paths;

    inline CarPathComponent(std::unordered_map<std::string, CarPath>&& paths)
        : paths(paths) {
    }

    inline CarPathComponent(const std::unordered_map<std::string, CarPath>& paths)
        : paths(paths) {
    }

    inline CarPathComponent(std::initializer_list<std::pair<const std::string, CarPath>> paths = {})
        : paths(paths) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<CarPathComponent>(entity, paths);
    }
};
