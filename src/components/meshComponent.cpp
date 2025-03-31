#include "components/meshComponent.hpp"

void MeshComponent::render(const MeshRenderData& renderData, MeshShadingMode shadingMode, Shader* shader) const {
    for (const auto& [_, mesh] : meshes) {
        mesh->render(renderData, shadingMode, shader);
    }

    for (const auto& [_, instancedMesh] : instancedMeshes) {
        MeshPtr mesh = instancedMesh.mesh;

        for (const auto& [name, instanceData] : instancedMesh.instances) {
            mesh->renderObjectInstanced<glm::mat4>(name, renderData, instanceData.instanceBuffer, shader);
        }
    }
}