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
void ResourceManager::loadResource<Shader>(const std::string& id, const std::string& vertexFilename, const std::string& fragmentFilename, const std::string& geometryFilename, bool instanced) {
    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;

    if (fragmentFilename.empty()) {
        vertexPath = resourceDir + vertexFilename + ".vert";
        fragmentPath = resourceDir + vertexFilename + ".frag";
        geometryPath = resourceDir + vertexFilename + ".geom";
    }
    else {
        vertexPath = resourceDir + vertexFilename;
        fragmentPath = resourceDir + fragmentFilename;
        geometryPath = resourceDir + geometryFilename;
    }

    ShaderProgram* shader = nullptr;
    if (!std::filesystem::exists(geometryPath) || geometryPath == resourceDir) {
        shader = new ShaderProgram(vertexPath, fragmentPath);
    }
    else {
        shader = new ShaderProgram(vertexPath, fragmentPath, geometryPath);
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

            // Shader* shader = new Shader();
            // if (filename.empty() && vertexPath.empty()) {
            //     // load shaders from child nodes
            //     const xml_node& defaultShaderNode = resourceNode.child("defaultShader");
            //     const std::string& defaultFilename = defaultShaderNode.attribute("filename").as_string();

            //     // load default shader
            //     if (defaultFilename.empty()) {
            //         vertexPath = defaultShaderNode.attribute("vertex").as_string();
            //         fragmentPath = defaultShaderNode.attribute("fragment").as_string();
            //         geometryPath = defaultShaderNode.attribute("geometry").as_string();

            //         if (std::filesystem::exists(resourceDir + geometryPath)) {
            //             shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath, resourceDir + geometryPath);
            //         }
            //         else {
            //             shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath);
            //         }
            //     }
            //     else {
            //         vertexPath = defaultFilename + ".vert";
            //         fragmentPath = defaultFilename + ".frag";

            //         if (std::filesystem::exists(resourceDir + defaultFilename + ".geom")) {
            //             geometryPath = defaultFilename + ".geom";
            //             shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath, resourceDir + geometryPath);
            //         }
            //         else {
            //             shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath);
            //         }
            //     }

            //     // load instanced shader
            //     const xml_node& instancedShaderNode = resourceNode.child("instancedShader");
            //     if (!instancedShaderNode.empty()) {
            //         const std::string& instancedFilename = instancedShaderNode.attribute("filename").as_string();
            //         std::string instancedVertexPath = instancedShaderNode.attribute("vertex").as_string();
            //         std::string instancedFragmentPath = instancedShaderNode.attribute("fragment").as_string();
            //         std::string instancedGeometryPath = instancedShaderNode.attribute("geometry").as_string();

            //         if (instancedFilename.empty()) {
            //             if (!instancedGeometryPath.empty() && std::filesystem::exists(resourceDir + instancedGeometryPath)) {
            //                 shader->instanced = new ShaderProgram(resourceDir + instancedVertexPath, resourceDir + instancedFragmentPath, resourceDir + instancedGeometryPath);
            //             }
            //             else {
            //                 shader->instanced = new ShaderProgram(resourceDir + instancedVertexPath, resourceDir + instancedFragmentPath);
            //             }
            //         }
            //         else {
            //             instancedVertexPath = instancedFilename + ".vert";
            //             instancedFragmentPath = instancedFilename + ".frag";

            //             if (std::filesystem::exists(resourceDir + instancedFilename + ".geom")) {
            //                 instancedGeometryPath = instancedFilename + ".geom";
            //                 shader->instanced = new ShaderProgram(resourceDir + instancedVertexPath, resourceDir + instancedFragmentPath, resourceDir + instancedGeometryPath);
            //             }
            //             else {
            //                 shader->instanced = new ShaderProgram(resourceDir + instancedVertexPath, resourceDir + instancedFragmentPath);
            //             }
            //         }
            //     }
            // }
            // else if (filename.empty()) {
            //     vertexPath = resourceNode.attribute("vertex").as_string();
            //     fragmentPath = resourceNode.attribute("fragment").as_string();
            //     geometryPath = resourceNode.attribute("geometry").as_string();

            //     if (!geometryPath.empty() && std::filesystem::exists(resourceDir + geometryPath)) {
            //         shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath, resourceDir + geometryPath);
            //     }
            //     else {
            //         shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath);
            //     }
            // }
            // else {
            //     vertexPath = filename + ".vert";
            //     fragmentPath = filename + ".frag";

            //     if (std::filesystem::exists(resourceDir + filename + ".geom")) {
            //         geometryPath = filename + ".geom";
            //         shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath, resourceDir + geometryPath);
            //     }
            //     else {
            //         shader->defaultShader = new ShaderProgram(resourceDir + vertexPath, resourceDir + fragmentPath);
            //     }
            // }

            // setResource<Shader>(id, ShaderPtr(shader));
        }
        else if (type == "texture") {
            bool rgba = resourceNode.attribute("rgba").as_bool();
            loadResource<Texture>(id, filename, rgba);
        }
        else if (type == "material") {
            if (filename.empty()) {
            }
            else {
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
        }
        else if (type == "mesh") {
            const std::string& shaderID = resourceNode.attribute("shader").as_string("MESH_SHADER");

            MeshPtr mesh = MeshLoader::loadMesh(resourceDir + filename);
            mesh->shader = getResource<Shader>(shaderID);

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
            pack->material = getResource<Material>(materialId);

            const RoadPackGeometry& geometries = RoadGeometryGenerator::generateRoadPackGeometries(RoadSpecs{roadwayWidth, roadwayHeight, sidewalkHeight, verticesPerCircle});
            for (const auto& [tileType, geometry] : geometries) {
                pack->roadGeometries.geometries[tileType].emplace_back(pack->material, geometry);
            }
            pack->roadGeometries.shader = getResource<Shader>(shaderId);

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
