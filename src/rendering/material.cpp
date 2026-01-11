#include "rendering/material.hpp"

#include "rendering/shader.hpp"

Material::Material() {
}

Material::Material(ResourcePtr<Texture> diffuse, ResourcePtr<Texture> specular)
    : diffuseTexture(diffuse), specularTexture(specular) {
}

Material::Material(ResourcePtr<Texture> diffuse, ResourcePtr<Texture> specular, float specularStrength, float shininess)
    : diffuseTexture(diffuse), specularTexture(specular), specularStrength(specularStrength), shininess(shininess) {
}

void Material::use(ShaderProgram* shader) const {
    // set textures
    ambientTexture->use(0);
    shader->setInt("material.ambientTexture", 0);

    diffuseTexture->use(1);
    shader->setInt("material.diffuseTexture", 1);

    specularTexture->use(2);
    shader->setInt("material.specularTexture", 2);

    // normal map
    if (normalMap) {
        normalMap->use(3);
        shader->setInt("material.normalMap", 3);
    }

    // properties
    shader->setFloat("material.shiniess", shininess);
    shader->setFloat("material.specularStrength", specularStrength);
    shader->setFloat("material.dissolve", dissolve);

}
