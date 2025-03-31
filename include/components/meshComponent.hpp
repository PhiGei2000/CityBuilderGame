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
#include "rendering/instancedMesh.hpp"
#include "resources/mesh.hpp"

#include <string>

struct MeshRenderData;

struct MeshComponent : public AssignableComponent {
    std::unordered_map<std::string, MeshPtr> meshes;
    std::unordered_map<std::string, InstancedMesh<glm::mat4>> instancedMeshes;

    inline MeshComponent() {
    }

    inline MeshComponent(const MeshPtr& meshes)
        : meshes({std::make_pair("", meshes)}) {
    }

    inline MeshComponent(std::unordered_map<std::string, MeshPtr> meshes,
                         std::unordered_map<std::string, InstancedMesh<glm::mat4>> instancedMeshes)
        : meshes{meshes}, instancedMeshes{instancedMeshes} {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<MeshComponent>(entity, meshes, instancedMeshes);
    }

    void render(const MeshRenderData& renderData, MeshShadingMode shadingMode, Shader* shader = nullptr) const;
};
