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
