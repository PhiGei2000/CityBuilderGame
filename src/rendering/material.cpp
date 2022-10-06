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

void Material::use(Shader* shader) const {
    bool useAmbientTexture = ambientTexture.get() != nullptr;
    bool useDiffuseTexture = diffuseTexture.get() != nullptr;
    bool useSpecularTexture = specularTexture.get() != nullptr;

    // set colors
    shader->setVector3("material.ambientColor", ambientColor);
    shader->setVector3("material.diffuseColor", diffuseColor);
    shader->setVector3("material.specularColor", specularColor);

    // set textures
    shader->setBool("material.useAmbientTexture", useAmbientTexture);
    if (useAmbientTexture) {
        ambientTexture->use(0);
        shader->setInt("material.ambientTexture", 0);
    }

    shader->setBool("material.useDiffuseTexture", useDiffuseTexture);
    if (useDiffuseTexture) {
        diffuseTexture->use(1);
        shader->setInt("material.diffuseTexture", 1);
    }

    shader->setBool("material.useSpecularTexture", useSpecularTexture);
    if (useSpecularTexture) {
        specularTexture->use(2);
        shader->setInt("material.specularTexture", 2);
    }

    // normal map
    if (normalMap) {
        normalMap->use(3);
        shader->setInt("material.normalMap", 3);
    }

    // properties
    shader->setFloat("material.shiniess", shininess);
    shader->setFloat("material.specularStrength", specularStrength);
    shader->setFloat("material.dissolve", dissolve);

    shader->setInt("material.illuminationModel", illuminationModel);
}
