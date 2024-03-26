/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
    TexturePtr ambientTexture = TexturePtr();
    TexturePtr diffuseTexture = TexturePtr();
    TexturePtr specularTexture = TexturePtr();
    TexturePtr normalMap = TexturePtr();

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
