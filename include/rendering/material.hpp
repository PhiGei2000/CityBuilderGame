#pragma once

#include "misc/typedefs.hpp"

#include "rendering/texture.hpp"
#include "resources/resourceManager.hpp"

#include <glm/glm.hpp>

class Shader;

struct Material {
    // colors
    glm::vec3 ambientColor = glm::vec3(1.0f);
    glm::vec3 diffuseColor = glm::vec3(1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f);

    // texture maps
    TexturePtr ambientTexture = TexturePtr(nullptr);
    TexturePtr diffuseTexture = TexturePtr(nullptr);
    TexturePtr specularTexture = TexturePtr(nullptr);

    // material properties
    float specularStrength = 0.8f; // = ((ior - 1)/(ior + 1))^2 / 0.08
    float shininess = 8.0f;
    float dissolve = 1.0f;

    // model of illumination 0 - 2
    int illuminationModel = 2;

    Material();
    Material(ResourcePtr<Texture> diffuse, ResourcePtr<Texture> specular);
    Material(ResourcePtr<Texture> diffuse, ResourcePtr<Texture> specular, float specularStrength, float shininess);

    void use(Shader* shader) const;
};

using MaterialPtr = ResourcePtr<Material>;
