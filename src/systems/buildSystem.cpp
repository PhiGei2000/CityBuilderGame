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
#include "resources/roadPack.hpp"

#include "misc/coordinateTransform.hpp"
#include "misc/ray.hpp"
#include "misc/utility.hpp"

#include "GLFW/glfw3.h"

#include <array>
#include <iostream>
#include <type_traits>

bool BuildSystem::GridMouseIntersection::positionUpdated() const {
    return intersection && positionChanged;
}

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

    eventDispatcher.sink<BuildingSelectedEvent>()
        .connect<&BuildSystem::handleBuildingSelectedEvent>(*this);
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
    if (building.buildingID != selectedBuildingID) {
        registry.destroy(currentBuilding);

        createNewBuilding();
    }

    if (gridMouseIntersection.positionUpdated() || buildingRotationUpdated) {
        if (building.buildingID.starts_with("infrastructure.road")) {
            if (game->getMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                BuildEvent event = BuildEvent(currentBuilding, {building.gridPosition}, building.buildingID, BuildShape::POINT);
                game->raiseEvent(event);

                createNewBuilding();
                building = registry.get<BuildingComponent>(currentBuilding);
            }
        }

        // update the transformation component of the current building, so it will be rendered at the right place
        building.gridPosition = gridMouseIntersection.position;

        // calculate position based on rotation
        TransformationComponent& transform = registry.get<TransformationComponent>(currentBuilding);
        glm::vec3 offset = getBuildingOffset(building.buildingID);

        // rotation | offset
        // ------------------
        // 0        | (0,0,0)
        // 1        | (0,0,1)
        // 2        | (1,0,1)
        // 3        | (1,0,0)

        switch (building.rotation) {
            case 1:
                offset.z += static_cast<float>(Configuration::cellSize) * building.size.x;
                break;
            case 2:
                offset.x += static_cast<float>(Configuration::cellSize) * building.size.x;
                offset.z += static_cast<float>(Configuration::cellSize) * building.size.y;
                break;
            case 3:
                offset.x += static_cast<float>(Configuration::cellSize) * building.size.y;
                break;
            default:
                break;
        }

        transform.setPosition(utility::normalizedWorldGridToWorldCoords(glm::vec2(building.gridPosition)) + offset);
        transform.calculateTransform();

        gridMouseIntersection.positionChanged = false;
        buildingRotationUpdated = false;
    }

    while (objectsToBuild.size() > 0) {
        entt::entity objectToBuild = objectsToBuild.front();

        BuildingComponent& building = registry.get<BuildingComponent>(objectToBuild);
        building.preview = false;

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
    bool x = size.x == 1;
    bool y = size.y == 1;

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

const glm::ivec2& BuildSystem::getDefaultSize(std::string buildingID) {
    const BuildableObjectPtr object = resourceManager.getResource<BuildableObject>(buildingID);

    return object->buildingInfo.defaultSize;
}

bool BuildSystem::canBuild(const std::vector<glm::ivec2>& positions, const std::string buildingID, const TerrainComponent& terrain) const {
    // TODO: Implement this when returning to non flat terrain

    return true;
}

const glm::vec3 BuildSystem::getBuildingOffset(const std::string& buildingID) {
    if (buildingID.starts_with("infrastructure.roads")) {
        return static_cast<float>(Configuration::cellSize) * glm::vec3(0.5f, 0.0f, 0.5f);
    }

    const BuildableObjectPtr object = resourceManager.getResource<BuildableObject>(buildingID);

    return object->buildingInfo.offset;
}

void BuildSystem::createNewBuilding() {
    glm::ivec2 defaultSize = glm::ivec2(1);
    glm::vec3 offset = glm::vec3(0.0f);

    if (selectedBuildingID.starts_with("infrastructure.roads")) {
        currentBuilding = registry.create();

        RoadPackPtr pack = resourceManager.getResource<RoadPack>(selectedBuildingID);
        MeshComponent& roadMesh = registry.emplace<MeshComponent>(currentBuilding, MeshPtr(new Mesh()));

        roadMesh.mesh->shader = pack->roadGeometries.shader;
        roadMesh.mesh->geometries[""] = pack->roadGeometries.geometries.at(RoadTileTypes::NOT_CONNECTED);
    }
    else {
        BuildableObjectPtr object = resourceManager.getResource<BuildableObject>(selectedBuildingID);
        currentBuilding = object->create(registry);
        defaultSize = object->buildingInfo.defaultSize;
    }

    registry.emplace<TransformationComponent>(currentBuilding, glm::vec3());
    registry.emplace<BuildingComponent>(currentBuilding, selectedBuildingID, glm::ivec2(0), 0, defaultSize, true);
}

void BuildSystem::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (game->getState() != GameState::BUILD_MODE) {
        return;
    }

    if (e.action != GLFW_RELEASE) {
        return;
    }

    if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
        // get the position of the build marker
        const BuildingComponent& building = registry.get<BuildingComponent>(currentBuilding);

        BuildShape shape = getShape(building.size);
        glm::ivec2 start;
        std::vector<glm::ivec2> positions = shape == BuildShape::POINT
                                                ? std::vector<glm::ivec2>{building.gridPosition}
                                                : std::vector<glm::ivec2>{building.gridPosition, building.gridPosition + glm::ivec2(glm::ceil(building.size - glm::vec2(1.0f)))};
        entt::entity entityToBuild = currentBuilding;

        bool canBuild;
        if (!building.buildingID.starts_with("infrastructure.road")) {
            objectsToBuild.emplace(currentBuilding);
        }
        createNewBuilding();

        BuildEvent event = BuildEvent(entityToBuild, positions, building.buildingID, shape);
        game->raiseEvent(event);
    }
    else if (e.button == GLFW_MOUSE_BUTTON_RIGHT) {
        auto [building, transform] = registry.get<BuildingComponent, TransformationComponent>(currentBuilding);
        building.rotation = (building.rotation + 1) % 4;

        float angle = glm::radians(building.rotation * 90.0f);
        float sin = glm::sin(angle / 2.0f);
        float cos = glm::cos(angle / 2.0f);
        transform.rotation = glm::quat(cos, 0.0f, sin, 0.0f);
        transform.calculateTransform();

        buildingRotationUpdated = true;
    }
}

void BuildSystem::handleMouseMoveEvent(const MouseMoveEvent& e) {
    if (game->getState() != GameState::BUILD_MODE)
        return;

    glm::vec2 mousePos = game->getMousePos();
    const auto& [intersection, position] = getGridPos(mousePos, getBuildingOffset(selectedBuildingID));

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
}

void BuildSystem::handleBuildingSelectedEvent(const BuildingSelectedEvent& e) {
    this->selectedBuildingID = e.buildingID;
}
