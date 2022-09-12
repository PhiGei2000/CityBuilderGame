#pragma once

#include "rendering/texture.hpp"
#include "resources/resourceManager.hpp"

struct Material {
    ResourcePtr<Texture> diffuse;
    ResourcePtr<Texture> specular;

    float shininess = 8.0f;
    float specularStrength = 0.8f;
};
