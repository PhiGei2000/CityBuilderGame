#include "resources/mesh.hpp"

Mesh::Mesh(ShaderPtr shader)
    : shader(shader) {

}

void Mesh::render(const glm::mat4& model) const {
    shader->use();
    shader->setMatrix4("model", model);

    for (const auto& [name, data] : geometries) {
        for (const auto& [material, geometry] : data) {
            if (material) {
                material->use(shader.get());
            }

            geometry->draw();
        }
    }
}
