#pragma once
#include "meshComponent.hpp"

#include "rendering/instancedMesh.hpp"

#include <glm/glm.hpp>

#include <vector>

// struct InstancedMeshComponent : public MeshComponent, public InstancedData<glm::mat4> {

//     inline InstancedMeshComponent(const MeshPtr& mesh, const std::vector<glm::mat4>& transformations)
//         : MeshComponent(mesh), InstancedData(transformations) {
//     }

//     inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
//         registry.emplace<InstancedMeshComponent>(entity, mesh, transformations);
//     }
// };

// struct MultiInstancedMeshComponent : public MeshComponent {
//     std::unordered_map<std::string, InstancedData<glm::mat4>> transforms;

//     inline MultiInstancedMeshComponent(const MeshPtr& mesh, const std::unordered_map<std::string, InstancedData<glm::mat4>>& instanceList)
//         : MeshComponent(mesh), transforms(instanceList) {
//     }

//     inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
//         registry.emplace<MultiInstancedMeshComponent>(entity, mesh, transforms);
//     }
// };
