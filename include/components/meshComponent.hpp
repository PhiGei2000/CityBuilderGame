#pragma once
#include "component.hpp"
#include "resources/mesh.hpp"

#include <string>

struct MeshComponent : public AssignableComponent {
    MeshPtr mesh;

    inline MeshComponent(const MeshPtr& mesh)
        : mesh(mesh) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<MeshComponent>(entity, mesh);
    }    
};
