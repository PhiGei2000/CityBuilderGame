#include "resources/resourceManager.hpp"

#include "misc/roads/roadSpecs.hpp"
#include "rendering/geometry.hpp"
#include "rendering/material.hpp"
#include "resources/mesh.hpp"
#include "resources/meshLoader.hpp"
#include "resources/roadGeometryGenerator.hpp"
#include "resources/roadPack.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <utility>

#include <pugixml.hpp>

using namespace pugi;

ResourceManager::ResourceManager(const std::string& resourceDir)
    : resourceDir(resourceDir) {

    loadResources();
}

ResourceManager::ResourceTypeException::ResourceTypeException(const char* message)
    : message(message) {
}

const char* ResourceManager::ResourceTypeException::what() const noexcept {
    return message;
}

template<typename T>
void ResourceManager::setResource(const std::string& id, ResourcePtr<T> data) {
    ResourcePtr<void> dataPtr = ResourcePtr<T>(data);

    if (resources.contains(id)) {
        resources[id].data.swap(dataPtr);
    }
    else {
        resources[id] = ResourceHolder{std::type_index(typeid(T)), dataPtr};
    }
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

        // if (type == "model") {
        //     loadResource<Geometry>(id, filename);
        // }
        // else
        if (type == "shader") {
            loadResource<Shader>(id, filename);
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
            const std::string& shaderId = resourceNode.attribute("shader").as_string("MESH_SHADER");

            ShaderPtr shader = getResource<Shader>(shaderId);

            MeshPtr mesh = MeshLoader::loadMesh(resourceDir + filename, shader);

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
            pack->roadGeometries = RoadGeometryGenerator::generateRoadPackGeometries(RoadSpecs{roadwayWidth, roadwayHeight, sidewalkHeight, verticesPerCircle});

            pack->shader = getResource<Shader>(shaderId);
            pack->material = getResource<Material>(materialId);

            setResource(id, ResourcePtr<RoadPack>(pack));
        }
    }
}

template<typename T>
std::shared_ptr<T> ResourceManager::getResource(const std::string& resourceId) const {
    const std::type_index type = std::type_index(typeid(T));
    const ResourceHolder& resource = resources.at(resourceId);

    if (type != resource.type) {
        std::string message = "Resource could not converted to ";

        throw ResourceTypeException(message.append(type.name()).c_str());
    }

    return std::reinterpret_pointer_cast<T>(resource.data);
}

template ResourcePtr<Texture> ResourceManager::getResource<Texture>(const std::string&) const;
template ResourcePtr<Shader> ResourceManager::getResource<Shader>(const std::string&) const;
// template ResourcePtr<Geometry> ResourceManager::getResource<Geometry>(const std::string&) const;
template ResourcePtr<RoadPack> ResourceManager::getResource<RoadPack>(const std::string&) const;
template ResourcePtr<Material> ResourceManager::getResource<Material>(const std::string&) const;
template ResourcePtr<Mesh> ResourceManager::getResource<Mesh>(const std::string&) const;

template void ResourceManager::setResource<Texture>(const std::string&, TexturePtr);
template void ResourceManager::setResource<Shader>(const std::string&, ShaderPtr);
// template void ResourceManager::setResource<Geometry>(const std::string&, Geometry*);
template void ResourceManager::setResource<RoadPack>(const std::string&, ResourcePtr<RoadPack>);
template void ResourceManager::setResource<Material>(const std::string&, MaterialPtr);
template void ResourceManager::setResource<Mesh>(const std::string&, MeshPtr);
