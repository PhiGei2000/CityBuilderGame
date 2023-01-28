#pragma once
#include "meshComponent.hpp"

#include "rendering/instanceBuffer.hpp"

#include <glm/glm.hpp>

#include <vector>

struct InstancedMeshComponent : public MeshComponent {
    std::vector<glm::vec3> offsets;
    InstanceBuffer instanceBuffer;

    inline InstancedMeshComponent(const MeshPtr& mesh, const std::vector<glm::vec3>& offsets)
        : MeshComponent(mesh), offsets(offsets) {
        instanceBuffer.fillBuffer(offsets);

        mesh->linkInstanceBuffer(instanceBuffer);        
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<InstancedMeshComponent>(entity, mesh, offsets);
    }
};
