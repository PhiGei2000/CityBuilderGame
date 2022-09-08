#pragma once

#include "rendering/texture.hpp"
#include "resources/resourceManager.hpp"

struct Material {
    ResourcePtr<Texture> diffuse;    
    ResourcePtr<Texture> specular;    
};
