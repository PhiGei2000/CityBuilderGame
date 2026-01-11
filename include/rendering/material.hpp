#pragma once

#include "misc/typedefs.hpp"

#include "rendering/texture.hpp"
#include "resources/resourceManager.hpp"

#include <glm/glm.hpp>

class ShaderProgram;

struct Material {
    // texture maps
    TexturePtr ambientTexture = TexturePtr();
    TexturePtr diffuseTexture = TexturePtr();
    TexturePtr specularTexture = TexturePtr();
    TexturePtr normalMap = TexturePtr();

    // material properties
    float specularStrength = 0.8f; // = ((ior - 1)/(ior + 1))^2 / 0.08
    float shininess = 8.0f;
    float dissolve = 1.0f;

    Material();
    Material(ResourcePtr<Texture> diffuse, ResourcePtr<Texture> specular);
    Material(ResourcePtr<Texture> diffuse, ResourcePtr<Texture> specular, float specularStrength, float shininess);

    void use(ShaderProgram* shader) const;
};

using MaterialPtr = ResourcePtr<Material>;
