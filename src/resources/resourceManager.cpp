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
#include "resources/resourceManager.hpp"

#include "misc/roads/roadSpecs.hpp"
#include "rendering/geometry.hpp"
#include "rendering/material.hpp"
#include "resources/mesh.hpp"
#include "resources/meshLoader.hpp"
#include "resources/objectLoader.hpp"
#include "resources/roadGeometryGenerator.hpp"
#include "resources/roadPack.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <utility>

#include <pugixml.hpp>

using namespace pugi;

ResourceManager::ResourceManager(const std::string& resourceDir)
    : resourceDir(resourceDir), objectLoader(*this) {

    loadResources();
}

ResourceManager::ResourceTypeException::ResourceTypeException(const char* message)
    : message(message) {
}

const char* ResourceManager::ResourceTypeException::what() const noexcept {
    return message;
}

template<>
void ResourceManager::loadResource<Shader>(const std::string& id, const std::string& filename) {
    const std::string& vertexPath = resourceDir + filename + ".vert";
    const std::string& fragmentPath = resourceDir + filename + ".frag";
    const std::string& geometryPath = resourceDir + filename + ".geom";

    Shader* shader;
    if (std::filesystem::exists(geometryPath)) {
        shader = new Shader(vertexPath, fragmentPath, geometryPath);
    }
    else {
        shader = new Shader(vertexPath, fragmentPath);
    }

    setResource(id, ShaderPtr(shader));
}

template<>
void ResourceManager::loadResource<Shader>(const std::string& id, const std::string& vertexPath, const std::string& fragmentPath) {
    Shader* shader = new Shader(resourceDir + vertexPath, resourceDir + fragmentPath);
    setResource(id, ShaderPtr(shader));
}

template<>
void ResourceManager::loadResource<Shader>(const std::string& id, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
    Shader* shader = new Shader(resourceDir + vertexPath, resourceDir + fragmentPath, resourceDir + geometryPath);
    setResource(id, ShaderPtr(shader));
}

template<>
void ResourceManager::loadResource<Texture>(const std::string& id, const std::string& filename, bool alpha) {
    Texture* texture = alpha ? new Texture(resourceDir + filename, GL_RGBA) : new Texture(resourceDir + filename);

    setResource(id, TexturePtr(texture));
}

void ResourceManager::loadResources() {
    xml_document doc;
    xml_parse_result result = doc.load_file((resourceDir + "resources.xml").c_str());

    if (!result) {
        std::cerr << "ResourceManager::loadResources "
                  << "cannot read xml file \"" << resourceDir << "\"" << std::endl;
        return;
    }

    for (const auto& resourceNode : doc.child("resources").children("resource")) {
        const std::string& type = resourceNode.attribute("type").as_string();

        const std::string& id = resourceNode.attribute("id").as_string();
        const std::string& filename = resourceNode.attribute("filename").as_string();

        if (type == "shader") {
            if (filename.empty()) {
                const std::string& vertexPath = resourceNode.attribute("vertex").as_string();
                const std::string& fragmentPath = resourceNode.attribute("fragment").as_string();
                const std::string& geometryPath = resourceNode.attribute("geometry").as_string();

                if (geometryPath.empty()) {
                    loadResource<Shader, const std::string&>(id, vertexPath, fragmentPath);
                }
                else {
                    loadResource<Shader, const std::string&, const std::string&>(id, vertexPath, fragmentPath, geometryPath);
                }
            }
            else {
                loadResource<Shader>(id, filename);
            }
        }
        else if (type == "texture") {
            bool rgba = resourceNode.attribute("rgba").as_bool();
            loadResource<Texture>(id, filename, rgba);
        }
        else if (type == "material") {
            std::unordered_map<std::string, MaterialPtr> materials = MeshLoader::loadMaterials(resourceDir + filename);

            for (const auto& materialNode : resourceNode.children("material")) {
                const std::string& materialName = materialNode.attribute("name").as_string();
                const std::string& materialId = materialNode.attribute("id").as_string();

                try {
                    MaterialPtr material = materials.at(materialName);

                    setResource(materialId, material);
                }
                catch (std::out_of_range e) {
                    std::cerr << "Material \"" << materialName << "\" not found in file \"" << filename << "\"" << std::endl;

                    throw e;
                }
            }
        }
        else if (type == "mesh") {
            MeshPtr mesh = MeshLoader::loadMesh(resourceDir + filename);

            setResource(id, mesh);
        }
        else if (type == "streetPack") {
            const std::string& shaderId = resourceNode.attribute("shader").as_string();
            const std::string& materialId = resourceNode.attribute("material").as_string();

            float roadwayWidth = resourceNode.attribute("roadwayWidth").as_float();
            float roadwayHeight = resourceNode.attribute("roadwayHeight").as_float();
            float sidewalkHeight = resourceNode.attribute("sidewalkHeight").as_float();
            unsigned int verticesPerCircle = resourceNode.attribute("verticesPerCircle").as_uint();

            RoadPack* pack = new RoadPack();
            pack->shader = getResource<Shader>(shaderId);
            pack->material = getResource<Material>(materialId);

            const RoadPackGeometry& geometries = RoadGeometryGenerator::generateRoadPackGeometries(RoadSpecs{roadwayWidth, roadwayHeight, sidewalkHeight, verticesPerCircle});
            for (const auto& [tileType, geometry] : geometries) {
                pack->roadGeometries.geometries[tileType].emplace_back(pack->material, geometry);
            }

            setResource(id, ResourcePtr<RoadPack>(pack));
        }
    }

    // objects
    std::string buildingsPath = resourceDir + "objects";
    for (const auto& entry : std::filesystem::directory_iterator(buildingsPath)) {
        if (entry.path().extension() == ".xml") {
            const std::string& id = "object." + entry.path().stem().string();
            ObjectPtr object = objectLoader.loadObject(entry.path().string());

            setResource(id, object);
        }
    }
}
