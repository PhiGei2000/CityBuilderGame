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

#include "rendering/instancedMesh.hpp"

#include <glm/glm.hpp>

#include <vector>

struct InstancedMeshComponent : public MeshComponent, public InstancedMesh<TransformationComponent> {

    inline InstancedMeshComponent(const MeshPtr& mesh, const std::vector<TransformationComponent>& transformations)
        : MeshComponent(mesh), InstancedMesh(transformations) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<InstancedMeshComponent>(entity, mesh, transformations);
    }
};

struct MultiInstancedMeshComponent : public MeshComponent {
    std::unordered_map<std::string, InstancedMesh<TransformationComponent>> transforms;

    inline MultiInstancedMeshComponent(const MeshPtr& mesh, const std::unordered_map<std::string, InstancedMesh<TransformationComponent>>& instanceList)
        : MeshComponent(mesh), transforms(instanceList) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<MultiInstancedMeshComponent>(entity, mesh, transforms);
    }
};

struct RoadRenderData;

struct RoadMeshComponent : public MeshComponent {
    using RoadTypeID = std::pair<RoadTypes, RoadTileType>;
    std::unordered_map<RoadTypeID, InstancedMesh<RoadRenderData>> roadMeshes;

    inline RoadMeshComponent(const MeshPtr& mesh, const std::unordered_map<RoadTypeID, InstancedMesh<RoadRenderData>>& instances)
        : MeshComponent(mesh), roadMeshes(instances) {
    }
};
