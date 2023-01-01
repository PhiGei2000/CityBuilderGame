#include "resources/mesh.hpp"

#include "misc/configuration.hpp"
#include "rendering/shadowBuffer.hpp"

Mesh::Mesh(ShaderPtr shader)
    : shader(shader) {
}

void Mesh::render(const glm::mat4& model, Shader* alternateShader) const {
    if (alternateShader == nullptr) {
        // use mesh shader
        shader->use();
        shader->setMatrix4("model", model);
        shader->setInt("shadowMaps", ShadowBuffer::depthMapOffset);

        for (const auto& [name, data] : geometries) {
            for (const auto& [material, geometry] : data) {
                if (material) {
                    material->use(shader.get());
                }

                geometry->draw();
            }
        }
    }
    else {
        alternateShader->use();
        alternateShader->setMatrix4("model", model);

        for (const auto& [_, data] : geometries) {
            for (const auto& [_, geometry] : data) {
                geometry->draw();
            }
        }
    }
}
