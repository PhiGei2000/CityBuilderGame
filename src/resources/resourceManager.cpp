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

#include "application.hpp"
#include "gui/menus/buildMenu.hpp"
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

ResourceManager::ResourceManager(const std::string& resourceDir, const Application* app)
    : resourceDir(resourceDir), objectLoader(*this), app(app) {

    loadResources();
}

ResourceManager::ResourceTypeException::ResourceTypeException(const char* message)
    : message(message) {
}

const char* ResourceManager::ResourceTypeException::what() const noexcept {
    return message;
}

template<>
void ResourceManager::loadResource<Shader>(const std::string& id, const std::string& vertexFilename, const std::string& fragmentFilename, const std::string& geometryFilename, bool instanced) {
    std::filesystem::path vertexPath;
    std::filesystem::path fragmentPath;
    std::filesystem::path geometryPath;

    if (fragmentFilename.empty()) {
        vertexPath = resourceDir/(vertexFilename + ".vert");
        fragmentPath = resourceDir/(vertexFilename + ".frag");
        geometryPath = resourceDir/(vertexFilename + ".geom");
    }
    else {
        vertexPath = resourceDir/vertexFilename;
        fragmentPath = resourceDir/fragmentFilename;
        geometryPath = resourceDir/geometryFilename;
    }

    ShaderProgram* shader = nullptr;
    if (!std::filesystem::exists(geometryPath) || geometryPath == resourceDir) {
        shader = new ShaderProgram(vertexPath.string(), fragmentPath.string());
    }
    else {
        shader = new ShaderProgram(vertexPath.string(), fragmentPath.string(), geometryPath.string());
    }

    bool resourceExists = resources.contains(id);
    ResourceHolder resource = resourceExists ? resources.at(id) : ResourceHolder{std::type_index(typeid(Shader)), std::shared_ptr<void>(new Shader())};
    if (instanced) {
        std::reinterpret_pointer_cast<Shader>(resource.data)->instanced = shader;
    }
    else {
        std::reinterpret_pointer_cast<Shader>(resource.data)->defaultShader = shader;
    }

    if (!resourceExists) {
        resources[id] = resource;
    }
}

template<>
void ResourceManager::loadResource<Texture>(const std::string& id, const std::string& filename, bool alpha) {
    std::filesystem::path texturePath = resourceDir/filename;
    Texture* texture = alpha ? new Texture(texturePath.string(), GL_RGBA) : new Texture(texturePath.string());

    setResource(id, TexturePtr(texture));
}

void ResourceManager::loadResources() {
    xml_document doc;
    xml_parse_result result = doc.load_file((resourceDir/"resources.xml").c_str());

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
            std::string vertexFilename = resourceNode.attribute("vertex").as_string();
            std::string fragmentFilename = resourceNode.attribute("fragment").as_string();
            std::string geometryFilename = resourceNode.attribute("geometry").as_string();

            if (filename.empty() && vertexFilename.empty()) {
                const xml_node& defaultShaderNode = resourceNode.child("defaultShader");
                if (!defaultShaderNode.empty()) {
                    const std::string& defaultFilename = defaultShaderNode.attribute("filename").as_string();

                    if (defaultFilename.empty()) {
                        const std::string& defaultVertexFilename = defaultShaderNode.attribute("vertex").as_string();
                        const std::string& defaultFragmentFilename = defaultShaderNode.attribute("fragment").as_string();
                        const std::string& defaultGeometryFilename = defaultShaderNode.attribute("geometry").as_string();

                        loadResource<Shader, const std::string&, const std::string&, bool>(id, defaultVertexFilename, defaultFragmentFilename, defaultGeometryFilename, false);
                    }
                    else {
                        loadResource<Shader, const std::string&, const std::string&, bool>(id, defaultFilename, "", "", false);
                    }
                }

                const xml_node& instancedShaderNode = resourceNode.child("instancedShader");
                if (!instancedShaderNode.empty()) {
                    const std::string& instancedFilename = instancedShaderNode.attribute("filename").as_string();

                    if (instancedFilename.empty()) {
                        const std::string& instancedVertexFilename = instancedShaderNode.attribute("vertex").as_string();
                        const std::string& instancedFragmentFilename = instancedShaderNode.attribute("fragment").as_string();
                        const std::string& instancedGeometryFilename = instancedShaderNode.attribute("geometry").as_string();

                        loadResource<Shader, const std::string&, const std::string&, bool>(id, instancedVertexFilename, instancedFragmentFilename, instancedGeometryFilename, true);
                    }
                    else {
                        loadResource<Shader, const std::string&, const std::string&, bool>(id, instancedFilename, "", "", true);
                    }
                }
            }
            else if (!vertexFilename.empty()) {
                loadResource<Shader, const std::string&, const std::string&, bool>(id, vertexFilename, fragmentFilename, geometryFilename, false);
            }
            else /*if (vertexFilename.empty())*/ {
                loadResource<Shader, const std::string&, const std::string&, bool>(id, filename, "", "", false);
            }
        }
        else if (type == "texture") {
            bool rgba = resourceNode.attribute("rgba").as_bool();
            loadResource<Texture>(id, filename, rgba);
        }
        else if (type == "material") {
            if (filename.empty()) {
            }
            else {
                std::unordered_map<std::string, MaterialPtr> materials = MeshLoader::loadMaterials((resourceDir/filename).string());

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
        }
        else if (type == "mesh") {
            const std::string& shaderID = resourceNode.attribute("shader").as_string("MESH_SHADER");

            MeshPtr mesh = MeshLoader::loadMesh((resourceDir/filename).string());
            mesh->shader = getResource<Shader>(shaderID);

            setResource(id, mesh);
        }
        else if (type == "roadPack") {
        }
    }

    // buildings
    std::filesystem::path buildingsPath = resourceDir/"buildings";
    std::filesystem::path roadsFile = buildingsPath/"roads.xml";

    result = doc.load_file(roadsFile.c_str());

    if (!result) {
        std::string message = "Failed to load file " + roadsFile.string();
        std::cout << message << std::endl;

        throw std::runtime_error(message);
    }

    BuildMenu* buildMenu = app->getGui()->getBuildMenu();
    for (const auto& node : doc.child("roads").children("roadPack")) {
        const std::string& id = node.attribute("id").as_string();
        const std::string& name = node.attribute("name").as_string();
        const std::string& icon = node.attribute("icon").as_string();
        const std::string& shaderId = node.attribute("shader").as_string();
        const std::string& materialId = node.attribute("material").as_string();

        const auto& specsNode = node.child("roadSpecs");
        RoadSpecs specs = {
            specsNode.attribute("roadwayWidth").as_float(),
            specsNode.attribute("roadwayHeight").as_float(),
            specsNode.attribute("sidewalkHeight").as_float(),
            specsNode.attribute("verticesPerCircle").as_uint()};

        MaterialPtr material = getResource<Material>(materialId);
        ShaderPtr shader = getResource<Shader>(shaderId);

        const std::string& buildingID = "infrastructure.roads." + id;
        RoadPack* pack = new RoadPack(specs, material, shader);
        setResource(buildingID, ResourcePtr<RoadPack>(pack));

        buildMenu->addBuildingEntry(BuildMenuEntry{name, "roads", BuildingCategory::INFRASTRUCTURE, buildingID, (resourceDir/icon).string()});
    }

    for (const auto& entry : std::filesystem::directory_iterator(buildingsPath)) {
        if (entry.path().extension() == ".xml") {
            if (entry.path().filename() == "roads.xml") {
                continue;
            }

            BuildMenuEntry menuEntry;
            ObjectPtr object = objectLoader.loadObject(entry.path().string(), &menuEntry);
            const std::string& id = object->buildable ? menuEntry.buildingID : "object." + object->name;

            if (object->buildable) {
                menuEntry.iconFilename = (resourceDir/menuEntry.iconFilename).string();
                buildMenu->addBuildingEntry(menuEntry);
            setResource<BuildableObject>(id, std::reinterpret_pointer_cast<BuildableObject>(object));
            }
            else {
                setResource(id, object);
            }
        }
    }
}
