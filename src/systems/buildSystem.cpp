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

#include "misc/coordinateTransform.hpp"
#include "misc/ray.hpp"
#include "misc/utility.hpp"

#include "GLFW/glfw3.h"

#include <array>
#include <iostream>

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
    if (game->getState() == GameState::BUILD_MODE) {
        BuildingComponent& building = registry.get<BuildingComponent>(currentBuilding);
        if (building.type != selectedBuildingType) {
            building.type = selectedBuildingType;
            building.size = getDefaultSize(selectedBuildingType);
            this->building = false;
        }

        if (gridMouseIntersection.intersection) {
            switch (building.type) {
                case BuildingType::ROAD:
                    if (this->building) {
                        const glm::ivec2& difference = gridMouseIntersection.position - building.gridPosition;

                        if (difference.x > difference.y) {
                            building.size = glm::ivec2(difference.x, 0);
                        }
                        else {
                            building.size = glm::ivec2(0, difference.y);
                        }
                    }
                    else {
                        building.gridPosition = gridMouseIntersection.position;
                    }
                    break;
                default:
                    building.gridPosition = gridMouseIntersection.position;
            }
        }
    }

BuildObjects:
    while (objectsToBuild.size() > 0) {
        entt::entity objectToBuild = objectsToBuild.front();

        BuildingComponent& building = registry.get<BuildingComponent>(objectToBuild);
        building.preview = false;

        registry.emplace<TransformationComponent>(objectToBuild, utility::normalizedWorldGridToWorldCoords(building.gridPosition), glm::vec3(0.0f, glm::radians(-90.0f) * building.rotation, 0.0f), glm::vec3(1.0f)).calculateTransform();

        std::vector<glm::ivec2> positions = {building.gridPosition, building.gridPosition + building.size};

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

// std::vector<glm::ivec2> BuildSystem::getRoadNodes(const glm::ivec2& start, const glm::ivec2& end) const {
// BuildShape shape = getShape(start, end);
// if (shape == BuildShape::LINE) {
// return {start, end};
//}
// else {
// glm::ivec2 node = state.xFirst ? glm::ivec2(start.x, end.y) : glm::ivec2(end.x, start.y);
// return {start, node, end};
//}
//}

bool BuildSystem::canBuild(const std::vector<glm::ivec2>& positions, const BuildingType type, const TerrainComponent& terrain) const {
    // if (type == BuildingType::ROAD) {
    //     const unsigned int segementsCount = positions.size() - 1;
    //     for (int segment = 0; segment < segementsCount; segment++) {
    //         glm::vec2 direction = positions[segment + 1] - positions[segment];
    //         const unsigned int edgeLength = glm::length(direction);
    //         direction = glm::normalize(direction);

    //         for (int i = 0; i <= edgeLength; i++) {
    //             const glm::vec2 position = Configuration::cellSize * glm::vec2(positions[segment] + i * glm::ivec2(direction));
    //             const TerrainSurfaceTypes surfaceType = terrain.getSurfaceType(position);

    //             if (i == 0 || i == edgeLength) {
    //                 if (surfaceType != TerrainSurfaceTypes::FLAT)
    //                     return false;
    //             }
    //             else {
    //                 if (surfaceType == TerrainSurfaceTypes::EDGE)
    //                     return false;
    //                 else if (surfaceType == TerrainSurfaceTypes::DIAGONAL) {
    //                     const glm::vec2 gradient = terrain.getSurfaceGradient(position);
    //                     if (glm::dot(gradient, direction) == 0) {
    //                         return false;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }

    return true;
}

constexpr glm::vec3 BuildSystem::getBuildmarkerOffset(const BuildingType type) {
    switch (type) {
        default:
            return glm::vec3(0.0f);
    }
}

void BuildSystem::createNewBuilding() {
    currentBuilding = registry.create();
    registry.emplace<BuildingComponent>(currentBuilding, selectedBuildingType, glm::vec2(), 0, getDefaultSize(selectedBuildingType), true);
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
        std::vector<glm::ivec2> positions = {building.gridPosition, building.gridPosition + building.size};

        bool canBuild;
        switch (building.type) {
            case BuildingType::ROAD:
                if (this->building) {
                    action = BuildAction::END;

                    objectsToBuild.emplace(currentBuilding);
                    createNewBuilding();
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

    const auto& [intersection, gridPos] = getGridPos(glm::vec2(e.x, e.y), getBuildmarkerOffset(selectedBuildingType));
    gridMouseIntersection = {intersection, gridPos};
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
