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

// template<>
// LightComponent ObjectLoader::loadComponent<LightComponent>(const xml_node& node) {
//
// }

template<>
MeshComponent ObjectLoader::loadComponent<MeshComponent>(const xml_node& node) {
    const std::string& filename = node.attribute("filename").as_string();

    MeshPtr mesh = MeshLoader::loadMesh(resourceManager.resourceDir + filename);
    return MeshComponent(mesh);
}

template<>
ParkingComponent ObjectLoader::loadComponent<ParkingComponent>(const xml_node& node) {
    std::vector<ParkingSpot> spots;

    float x, y, z;
    for (const xml_node& parkingSpotNode : node.children("parkingSpot")) {
        const std::string& id = parkingSpotNode.attribute("id").as_string();
        const std::string& position = parkingSpotNode.attribute("position").as_string();

        std::stringstream posStream(position);
        posStream >> x;
        posStream >> y;
        posStream >> z;
        spots.emplace_back(id, glm::vec3(x, y, z));
    }

    return ParkingComponent(spots);
}

template<>
CarPathComponent ObjectLoader::loadComponent<CarPathComponent>(const xml_node& node) {
    std::unordered_map<std::string, CarPath> paths;

    float x, y, z;
    for (const xml_node& pathNode : node.children("path")) {
        const std::string& id = pathNode.attribute("id").as_string();
        const std::string& direction = pathNode.attribute("direction").as_string();

        std::vector<glm::vec3> positions;
        for (const xml_node& positionNode : pathNode.children("node")) {
            const std::string& position = positionNode.attribute("position").as_string();

            std::stringstream posStream(position);
            posStream >> x;
            posStream >> y;
            posStream >> z;
            positions.emplace_back(glm::vec3(x, y, z));
        }

        if (direction == "in") {
            paths[id].pathIn = positions;
        }
        else if (direction == "out") {
            paths[id].pathOut = positions;
        }
    }

    return CarPathComponent(paths);
}

template<>
VelocityComponent ObjectLoader::loadComponent<VelocityComponent>(const xml_node& node) {
    return VelocityComponent();
}

ObjectPtr ObjectLoader::loadObject(const std::string& filename) {
    xml_document doc;
    xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        std::string message = "Failed to load file " + filename;
        std::cout << message << std::endl;

        throw std::runtime_error(message);
    }

    Object* object = new Object();
    for (const auto& node : doc.child("object")) {
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
        else if (name == "car") {
            object->addComponent<CarComponent>(loadComponent<CarComponent>(node));
        }
        else if (name == "carPath") {
            object->addComponent<CarPathComponent>(loadComponent<CarPathComponent>(node));
        }
        else if (name == "velocity") {
            object->addComponent<VelocityComponent>(loadComponent<VelocityComponent>(node));
        }
    }

    return ObjectPtr(object);
}
