#include "rendering/meshRenderData.hpp"

void MeshRenderData::uploadToShader(ShaderProgram* shader) const {
    shader->use();

    shader->setMatrix4("model", model);
    shader->setBool("preview", preview);
    shader->setInt("shadowMaps", shadowMaps);
}