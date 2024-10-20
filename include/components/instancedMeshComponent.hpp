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
#include "meshComponent.hpp"

#include "misc/roads/roadTile.hpp"
#include "misc/roads/roadTypes.hpp"

#include "rendering/instancedMesh.hpp"

#include <glm/glm.hpp>

#include <vector>

struct InstancedMeshComponent : public MeshComponent, public InstancedMesh<glm::mat4> {

    inline InstancedMeshComponent(const MeshPtr& mesh, const std::vector<glm::mat4>& transformations)
        : MeshComponent(mesh), InstancedMesh(transformations) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<InstancedMeshComponent>(entity, mesh, transformations);
    }
};

struct MultiInstancedMeshComponent : public MeshComponent {
    std::unordered_map<std::string, InstancedMesh<glm::mat4>> transforms;

    inline MultiInstancedMeshComponent(const MeshPtr& mesh, const std::unordered_map<std::string, InstancedMesh<glm::mat4>>& instanceList)
        : MeshComponent(mesh), transforms(instanceList) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<MultiInstancedMeshComponent>(entity, mesh, transforms);
    }
};

