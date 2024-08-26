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
#include "resources/objectLoader.hpp"

#include "components/components.hpp"
#include "gui/menus/buildMenu.hpp"
#include "resources/meshLoader.hpp"
#include "resources/resourceManager.hpp"

#include <iostream>

const std::unordered_map<std::string, ObjectLoader::NodeNames> ObjectLoader::nodeNames = {
    std::make_pair("name", NodeNames::NAME),
    std::make_pair("mesh", NodeNames::MESH),
    std::make_pair("parking", NodeNames::PARKING),
    std::make_pair("car", NodeNames::CAR),
    std::make_pair("path", NodeNames::PATH),
    std::make_pair("velocity", NodeNames::VELCOITY),
    std::make_pair("buildable", NodeNames::BUILDABLE)};

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
    const std::string& shaderID = node.attribute("shader").as_string();

    MeshPtr mesh = MeshLoader::loadMesh(resourceManager.resourceDir/filename);
    mesh->shader = resourceManager.getResource<Shader>(shaderID);

    return MeshComponent(mesh);
}

template<>
ParkingComponent ObjectLoader::loadComponent<ParkingComponent>(const xml_node& node) {
    std::vector<ParkingSpot> spots;

    float x, y, z;
    for (const xml_node& parkingSpotNode : node.children("parkingSpot")) {
        const std::string& id = parkingSpotNode.attribute("id").as_string();
        const std::string& position = parkingSpotNode.attribute("position").as_string();
        const std::string& direction = parkingSpotNode.attribute("direction").as_string("NORTH");
        const bool occupied = parkingSpotNode.attribute("occupied").as_bool(false);

        static const std::unordered_map<std::string, Direction> directions = {
            std::make_pair("NORTH", Direction::NORTH),
            std::make_pair("EAST", Direction::EAST),
            std::make_pair("SOUTH", Direction::SOUTH),
            std::make_pair("WEST", Direction::WEST)};

        Direction dir;
        try {
            dir = directions.at(direction);
        }
        catch (std::out_of_range) {
            dir = Direction::UNDEFINED;
        }

        std::stringstream posStream(position);
        posStream >> x;
        posStream >> y;
        posStream >> z;
        spots.emplace_back(id, glm::vec3(x, y, z), dir, occupied);
    }

    return ParkingComponent(spots);
}

template<>
PathComponent ObjectLoader::loadComponent<PathComponent>(const xml_node& node) {
    std::unordered_map<std::string, Path> paths;

    float x, y, z;
    for (const xml_node& pathNode : node.children("path")) {
        const std::string& id = pathNode.attribute("id").as_string();
        const std::string& direction = pathNode.attribute("direction").as_string();

        Path path;
        for (const xml_node& positionNode : pathNode.children("node")) {
            const std::string& position = positionNode.attribute("position").as_string();

            std::stringstream posStream(position);
            posStream >> x;
            posStream >> y;
            posStream >> z;
            path.add(glm::vec3(x, y, z));
        }

        paths[id] = Path(path);
    }

    return PathComponent(paths);
}

template<>
VelocityComponent ObjectLoader::loadComponent<VelocityComponent>(const xml_node& node) {
    return VelocityComponent();
}

void ObjectLoader::loadBuildMenu(const xml_node& node) const {
}

ObjectPtr ObjectLoader::loadObject(const std::string& filename, BuildMenuEntry* entry) {
    xml_document doc;
    xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        std::string message = "Failed to load file " + filename;
        std::cout << message << std::endl;

        throw std::runtime_error(message);
    }

    bool buildable = !(doc.child("object").children("buildable").empty());

    Object* object = buildable ? new BuildableObject("") : new Object();
    for (const auto& node : doc.child("object")) {
        const std::string& name = node.name();
        const NodeNames nodeName = nodeNames.at(name);

        switch (nodeName) {
            case NodeNames::NAME:
                object->name = node.text().as_string();
                break;
            case NodeNames::MESH:
                object->addComponent<MeshComponent>(loadComponent<MeshComponent>(node));
                break;
            case NodeNames::PARKING:
                object->addComponent<ParkingComponent>(loadComponent<ParkingComponent>(node));
                break;
            case NodeNames::CAR:
                object->addComponent<CarComponent>(loadComponent<CarComponent>(node));
                break;
            case NodeNames::PATH:
                object->addComponent<PathComponent>(loadComponent<PathComponent>(node));
                break;
            case NodeNames::VELCOITY:
                object->addComponent<VelocityComponent>(loadComponent<VelocityComponent>(node));
                break;
            case NodeNames::BUILDABLE: {
                BuildableObject* buildable = reinterpret_cast<BuildableObject*>(object);

                const std::string& buildingID = node.attribute("id").as_string();
                const std::string& category = node.attribute("category").as_string();
                const std::string& section = node.attribute("section").as_string();
                const std::string& iconFilename = node.attribute("iconFilename").as_string();

                buildable->buildingInfo.buildingID = category + "." + buildingID;

                entry->name = object->name;
                entry->category = getBuildingCategory(category);
                entry->section = section;
                entry->buildingID = buildable->buildingInfo.buildingID;
                entry->iconFilename = iconFilename;

                std::stringstream defaultSize(node.attribute("defaultSize").as_string("1 1"));

                defaultSize >> buildable->buildingInfo.defaultSize.x;
                defaultSize >> buildable->buildingInfo.defaultSize.y;

                std::stringstream offset(node.attribute("offset").as_string("0 0 0"));
                offset >> buildable->buildingInfo.offset.x;
                offset >> buildable->buildingInfo.offset.y;
                offset >> buildable->buildingInfo.offset.z;
            } break;
            default:
                std::cout << "Undefined node name \"" << name << "\" in file " << filename << std::endl;
                break;
        }
    }

    return ObjectPtr(object);
}
