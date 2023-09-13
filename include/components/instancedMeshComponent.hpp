#pragma once
#include "meshComponent.hpp"
#include "transformationComponent.hpp"

#include "rendering/instanceBuffer.hpp"

#include <glm/glm.hpp>

#include <vector>

struct InstancedMeshComponent : public MeshComponent {
    std::vector<TransformationComponent> transformations;
    InstanceBuffer instanceBuffer;

    inline InstancedMeshComponent(const MeshPtr& mesh, const std::vector<TransformationComponent>& transformations)
        : MeshComponent(mesh), transformations(transformations) {
        instanceBuffer.fillBuffer(transformations);
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<InstancedMeshComponent>(entity, mesh, transformations);
    }
};
