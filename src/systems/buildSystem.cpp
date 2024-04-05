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
#include "systems/buildSystem.hpp"

#include "components/components.hpp"
#include "events/events.hpp"

#include "resources/object.hpp"

#include "misc/coordinateTransform.hpp"
#include "misc/ray.hpp"
#include "misc/utility.hpp"

#include "GLFW/glfw3.h"

#include <array>
#include <iostream>
#include <type_traits>

BuildSystem::BuildSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<MouseButtonEvent>()
        .connect<&BuildSystem::handleMouseButtonEvent>(*this);

    eventDispatcher.sink<KeyEvent>()
        .connect<&BuildSystem::handleKeyEvent>(*this);

    eventDispatcher.sink<BuildEvent>()
        .connect<&BuildSystem::handleBuildEvent>(*this);

    eventDispatcher.sink<MouseMoveEvent>()
        .connect<&BuildSystem::handleMouseMoveEvent>(*this);
}

void BuildSystem::init() {
    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();

    createNewBuilding();
}

void BuildSystem::update(float dt) {
    if (game->getState() != GameState::BUILD_MODE) {
        return;
    }

    BuildingComponent& building = registry.get<BuildingComponent>(currentBuilding);
    if (building.type != selectedBuildingType) {
        registry.destroy(currentBuilding);

        createNewBuilding();
    }

    if (gridMouseIntersection.intersection) {
        if (gridMouseIntersection.positionChanged) {
            switch (building.type) {
                case BuildingType::ROAD: {
                    RoadComponent& road = registry.get<RoadComponent>(currentBuilding);
                    road.clear();
                    const glm::ivec2& difference = building.gridPosition - gridMouseIntersection.position;

                    if (this->building) {
                        // first node already placed
                        road.clear();
                        if (difference.x > difference.y) {
                            building.size = glm::ivec2(difference.x, 0);
                        }
                        else {
                            building.size = glm::ivec2(0, difference.y);
                        }

                        // update road graph
                        // road.graph.insertNode(building.gridPosition);
                        // road.graph.insertNode(building.gridPosition + glm::ivec2(building.size));
                        // road.graph.connectNodes(building.gridPosition, building.gridPosition + glm::ivec2(building.size));
                    }
                    else {
                        building.gridPosition = gridMouseIntersection.position;
                        // road.graph.insertNode(building.gridPosition);
                    }

                    road.meshOutdated = true;
                } break;
                default:
                    // update the transformation component of the current building, so it will be rendered at the right place
                    building.gridPosition = gridMouseIntersection.position;
                    TransformationComponent& transform = registry.get<TransformationComponent>(currentBuilding);
                    // calculate rotation
                    float angle = building.rotation / 4.0f * glm::pi<float>();
                    float sin = glm::sin(angle);
                    float cos = glm::cos(angle);
                    transform.rotation = glm::quat(cos, 0.0f, sin, 0.0f);

                    // calculate position based on rotation

                    // rotation | offset
                    // ------------------
                    // 0        | (0,0,0)
                    // 1        | (1,0,0)
                    // 2        | (1,0,1)
                    // 3        | (0,0,1)

                    // x-direction: offset.x = 1/2(1 + sin(pi/2 r) - \cos(pi/2 r)) = \sin(pi/4 r) * (\sin(pi/4 * r) + \cos(pi/4 r))
                    // z-direction: offset.y = 1/2(1 - sin(pi/2 r) - cos(pi/2 r)) = sin(pi/4 r) * (sin(pi/4 * r) - cos(pi/4 r))
                    glm::vec2 offset = sin * glm::vec2(sin + cos, sin - cos) * building.size;
                    transform.setPosition(utility::normalizedWorldGridToWorldCoords(glm::vec2(building.gridPosition) + offset));

                    transform.calculateTransform();
            }
            gridMouseIntersection.positionChanged = false;
        }
    }

    while (objectsToBuild.size() > 0) {
        entt::entity objectToBuild = objectsToBuild.front();

        BuildingComponent& building = registry.get<BuildingComponent>(objectToBuild);
        building.preview = false;

        std::vector<glm::ivec2> positions = {building.gridPosition, building.gridPosition + glm::ivec2(glm::ceil(building.size))};

        BuildEvent event(positions, building.type, BuildAction::ENTITY_CREATED, getShape(building.size));
        game->raiseEvent(event);

        objectsToBuild.pop();
    }
}

std::pair<bool, glm::ivec2> BuildSystem::getGridPos(const glm::vec2& mousePos, const glm::vec3& offset) const {
    const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(cameraEntity);
    const glm::vec3& cameraPos = cameraTransform.position + offset;

    glm::vec4 ray_clip = glm::vec4(mousePos, -1.0f, 1.0f);
    glm::vec4 ray_eye = glm::inverse(camera.projectionMatrix) * ray_clip;
    ray_eye.z = -1;
    ray_eye.w = 0;

    glm::vec3 direction = glm::vec3(glm::inverse(camera.viewMatrix) * ray_eye);

    Ray ray(cameraPos, direction);
    const auto& cells = ray.getCellIntersections(11 * Configuration::cellSize);

    // get cell with first intersection (not perfect yet)
    int i = 0;
    while (i < cells.size()) {
        const auto& [cell, intersection] = cells[i];
        if (!game->terrain.positionValid(cell)) {
            break;
        }

        float terrainHeight = game->terrain.getTerrainHeight(cell);
        if (intersection.y < terrainHeight) {
            return std::make_pair(true, i > 0 ? cells[i - 1].first : cell);
        }

        i++;
    }

    return std::make_pair(false, glm::ivec2());
}

constexpr BuildShape BuildSystem::getShape(const glm::ivec2& size) {
    bool x = size.x == 0;
    bool y = size.y == 0;

    if (x && y) {
        return BuildShape::POINT;
    }
    else if (x || y) {
        return BuildShape::LINE;
    }
    else {
        return BuildShape::AREA;
    }
}

constexpr glm::ivec2 BuildSystem::getDefaultSize(BuildingType type) {
    switch (type) {
        case BuildingType::PARKING_LOT:
            return glm::ivec2(1);
        default:
            return glm::ivec2(0);
    }
}

bool BuildSystem::canBuild(const std::vector<glm::ivec2>& positions, const BuildingType type, const TerrainComponent& terrain) const {
    // TODO: Implement this when returning to non flat terrain

    return true;
}

constexpr glm::vec3 BuildSystem::getBuildmarkerOffset(const BuildingType type) {
    switch (type) {
        default:
            return glm::vec3(0.0f);
    }
}

void BuildSystem::createNewBuilding() {
    static std::string objectIDs[] = {
        "",                   // NONE
        "",                   // CLEAR
        "",                   // ROAD
        "object.parking_lot", // PARKING_LOT
    };
    glm::vec3 position;

    switch (selectedBuildingType) {
        case BuildingType::ROAD: {
            currentBuilding = registry.create();

            const RoadComponent& road = registry.emplace<RoadComponent>(currentBuilding);
            // Mesh* roadMesh = new Mesh();
            RoadMeshComponent roadMesh = RoadMeshComponent();
            // roadMesh->geometries["BASIC_STREETS"] = {std::make_pair(resourceManager.getResource<Material>("BASIC_STREET_MATERIAL"), GeometryPtr(new MeshGeometry()))};

            // registry.emplace<MeshComponent>(currentBuilding, MeshPtr(roadMesh));
        } break;
        case BuildingType::CLEAR:
        case BuildingType::NONE:
            currentBuilding = registry.create();
            break;
        default: {
            const std::string& id = "object." + getBuildingName(selectedBuildingType);

            ObjectPtr object = resourceManager.getResource<Object>(id);
            currentBuilding = object->create(registry);
        } break;
    }

    registry.emplace<TransformationComponent>(currentBuilding, glm::vec3());
    registry.emplace<BuildingComponent>(currentBuilding, selectedBuildingType, glm::ivec2(0), 0, getDefaultSize(selectedBuildingType), true);
}

void BuildSystem::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (e.action != GLFW_RELEASE) {
        return;
    }

    if (game->getState() != GameState::BUILD_MODE) {
        return;
    }

    if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
        // get the position of the build marker
        const BuildingComponent& building = registry.get<BuildingComponent>(currentBuilding);

        BuildAction action = BuildAction::DEFAULT;
        BuildShape shape = BuildShape::POINT;
        glm::ivec2 start;
        std::vector<glm::ivec2> positions = {building.gridPosition, building.gridPosition + glm::ivec2(glm::ceil(building.size))};

        bool canBuild;
        switch (building.type) {
            case BuildingType::ROAD:
                if (this->building) {
                    action = BuildAction::END;

                    objectsToBuild.emplace(currentBuilding);
                    createNewBuilding();
                    this->building = false;
                }
                else {
                    this->building = true;
                }
                break;
            case BuildingType::PARKING_LOT:
                action = BuildAction::END;

                objectsToBuild.emplace(currentBuilding);
                createNewBuilding();
                break;
            default:
                break;
        }

        BuildEvent event = BuildEvent(positions, building.type, action, shape);
#if DEBUG
        static std::string actionNames[] = {"DEFAULT", "BEGIN", "END"};

        std::cout << "Raising Build Event: {pos: (" << positions[0] << "), type: " << building.type << ", action: " << actionNames[(unsigned int)action] << ", start: (" << start << ")}" << std::endl;
#endif
        game->raiseEvent(event);
    }
    else if (e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        BuildingComponent building = registry.get<BuildingComponent>(currentBuilding);
        building.rotation++;
    }
}

void BuildSystem::handleMouseMoveEvent(const MouseMoveEvent& e) {
    if (game->getState() != GameState::BUILD_MODE)
        return;

    glm::vec2 mousePos = game->getMousePos();
    const auto& [intersection, position] = getGridPos(mousePos, getBuildmarkerOffset(selectedBuildingType));

    gridMouseIntersection.intersection = intersection;

    if (intersection) {
        gridMouseIntersection.positionChanged |= glm::any(glm::notEqual(position, gridMouseIntersection.position));
        gridMouseIntersection.position = position;
    }
}

void BuildSystem::handleKeyEvent(const KeyEvent& e) {
    if (e.action != GLFW_PRESS)
        return;

    GameState state = game->getState();
    if (e.key == GLFW_KEY_B) {
        if (state == GameState::RUNNING) {
            game->setState(GameState::BUILD_MODE);
        }
        else if (state == GameState::BUILD_MODE) {
            game->setState(GameState::RUNNING);
        }
    }
    else if (e.key == GLFW_KEY_ESCAPE) {
        if (state == GameState::BUILD_MODE) {
            game->setState(GameState::RUNNING);
        }
    }
}

void BuildSystem::handleBuildEvent(const BuildEvent& e) {
    if (e.action != BuildAction::SELECT) {
        return;
    }

#if DEBUG
    std::cout << "Build type selected: " << e.type << std::endl;
#endif

    // setState(e.type, state.currentPosition, false);
    selectedBuildingType = e.type;
}
