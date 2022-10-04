#include "resources/objectLoader.hpp"

#include "components/components.hpp"
#include "resources/meshLoader.hpp"
#include "resources/resourceManager.hpp"

#include <iostream>

using namespace pugi;

ObjectLoader::ObjectLoader(ResourceManager& resourceManager)
    : resourceManager(resourceManager) {
}

template<>
CarComponent ObjectLoader::loadComponent<CarComponent>(const xml_node& node) {
    return CarComponent();
}

template<>
LightComponent ObjectLoader::loadComponent<LightComponent>(const xml_node& node) {
}

template<>
MeshComponent ObjectLoader::loadComponent<MeshComponent>(const xml_node& node) {
    const std::string& filename = node.attribute("filename").as_string();
    const std::string& shaderID = node.attribute("shader").as_string();

    ShaderPtr shader = resourceManager.getResource<Shader>(shaderID);

    MeshPtr mesh = MeshLoader::loadMesh(resourceManager.resourceDir + filename, shader);
    return MeshComponent(mesh);
}

template<>
ParkingComponent ObjectLoader::loadComponent<ParkingComponent>(const xml_node& node) {
    std::vector<ParkingSpot> spots;

    float x, y, z;
    for (const xml_node& parkingSpotNode : node.children("parkingSpot")) {
        const std::string& position = parkingSpotNode.attribute("position").as_string();

        std::stringstream posStream(position);
        posStream >> x;
        posStream >> y;
        posStream >> z;
        spots.emplace_back(glm::vec3(x,y,z));
    }

    return ParkingComponent(spots);
}

ObjectPtr ObjectLoader::loadBuilding(const std::string& filename) {
    xml_document doc;
    xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        std::string message = "Failed to load file " + filename;
        std::cout << message << std::endl;

        throw std::runtime_error(message);
    }

    Object* object = new Object();
    for (const auto& node : doc.child("building")) {
        const std::string& name = node.name();

        if (name == "name") {
            object->name = node.text().as_string();
        }
        else if (name == "mesh") {
            object->addComponent<MeshComponent>(loadComponent<MeshComponent>(node));
        }
        else if (name == "parking") {
            object->addComponent<ParkingComponent>(loadComponent<ParkingComponent>(node));
        }
    }

    return ObjectPtr(object);
}
