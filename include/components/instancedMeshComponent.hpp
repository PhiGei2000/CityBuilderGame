#pragma once
#include "meshComponent.hpp"

#include "rendering/instancedMesh.hpp"

#include <glm/glm.hpp>

#include <vector>

struct InstancedMeshComponent : public MeshComponent, public InstancedMesh {

    inline InstancedMeshComponent(const MeshPtr& mesh, const std::vector<TransformationComponent>& transformations)
        : MeshComponent(mesh), InstancedMesh(transformations) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<InstancedMeshComponent>(entity, mesh, transformations);
    }
};

struct MultiInstancedMeshComponent : public MeshComponent {
    std::unordered_map<std::string, InstancedMesh> transforms;

    inline MultiInstancedMeshComponent(const MeshPtr& mesh, const std::unordered_map<std::string, InstancedMesh>& instanceList)
        : MeshComponent(mesh), transforms(instanceList) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<MultiInstancedMeshComponent>(entity, mesh, transforms);
    }
};
