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
}

void BuildSystem::init() {
    cameraEntity = registry.view<CameraComponent, TransformationComponent>().front();

    buildMarkerEntity = registry.create();
    registry.emplace<BuildMarkerComponent>(buildMarkerEntity, glm::ivec2(0.0f), false);
    registry.emplace<TransformationComponent>(buildMarkerEntity, glm::vec3(0.0f), glm::quat(), glm::vec3(1.0f));

    registry.emplace<MeshComponent>(buildMarkerEntity, resourceManager.getResource<Mesh>("BUILDMARKER_MESH"));
}

void BuildSystem::update(float dt) {
    BuildMarkerComponent& buildMarkerComponent = registry.get<BuildMarkerComponent>(buildMarkerEntity);
    TransformationComponent& transform = registry.get<TransformationComponent>(buildMarkerEntity);

    if (game->getState() == GameState::BUILD_MODE) {
        // get grid position
        const glm::vec2& mousePos = game->getMousePos();
        const glm::vec2& buildMarkerOffset = getBuildmarkerOffset(state.selectedBuildingType);

        const auto& [intersection, gridPos] = getGridPos(mousePos, -glm::vec3(Configuration::cellSize * buildMarkerOffset.x, 0.0f, Configuration::cellSize * buildMarkerOffset.y));

        // display build marker and update the build marker position
        buildMarkerComponent.visible = true;

        if (intersection) {
            if (gridPos != buildMarkerComponent.position) {
                if (!game->terrain.positionValid(gridPos)) {
                    goto BuildObjects;
                }

                buildMarkerComponent.position = gridPos;
                const glm::vec2& offset = static_cast<float>(Configuration::cellSize) * (glm::vec2(gridPos) + buildMarkerOffset);
                // set height
                transform.position = glm::vec3(offset.x, game->terrain.getTerrainHeight(gridPos) + 0.1f, offset.y);
                transform.calculateTransform();

                if (this->state.building) {
                    // render building preview
                    BuildEvent event = BuildEvent{
                        {gridPos, state.startPosition},
                        state.selectedBuildingType,
                        BuildAction::PREVIEW,
                        getShape(state.startPosition, gridPos)
                    };

                    if (state.selectedBuildingType == BuildingType::ROAD) {
                        event.positions = getRoadNodes(state.startPosition, gridPos);
                        event.shape = BuildShape::LINE;
                    }

                    const TerrainComponent& terrain = registry.get<TerrainComponent>(registry.view<TerrainComponent>().front());
                    event.valid = this->canBuild(event.positions, event.type, terrain);

                    game->raiseEvent(event);
                }
            }
        }
        else {
            buildMarkerComponent.visible = false;
        }
    }
    else {
        buildMarkerComponent.visible = false;
    }

BuildObjects:
    while (objectsToBuild.size() > 0) {
        BuildInfo objectToBuild = objectsToBuild.front();

        entt::entity entity = objectToBuild.object->create(registry);
        registry.emplace<TransformationComponent>(entity, utility::normalizedWorldGridToWorldCoords(objectToBuild.positions[0]), glm::vec3(0.0f, glm::radians(-90.0f) * (int)objectToBuild.direction, 0.0f), glm::vec3(1.0f)).calculateTransform();
        registry.emplace<BuildingComponent>(entity, objectToBuild.positions[0]);

        BuildEvent event(objectToBuild.positions, objectToBuild.type, BuildAction::ENTITY_CREATED, getShape(objectToBuild.positions[0], objectToBuild.positions[1]));
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

void BuildSystem::setState(BuildingType selectedType, const glm::ivec2& currentPosition, bool building, const glm::ivec2& startPosition, bool xFirst) {
    // update state variables
    state.building = building;
    state.currentPosition = currentPosition;
    state.startPosition = building ? startPosition : glm::ivec2(-INT_MAX);
    state.selectedBuildingType = selectedType;
    state.xFirst = xFirst;

    if (state.building) {
        std::vector<glm::ivec2> positions;
        BuildShape shape;
        if (state.selectedBuildingType == BuildingType::ROAD) {
            positions = getRoadNodes(startPosition, currentPosition);
            shape = BuildShape::LINE;
        }
        else {
            positions = {startPosition, currentPosition};
            shape = getShape(startPosition, currentPosition);
        }

        BuildEvent event = BuildEvent(positions, state.selectedBuildingType, BuildAction::PREVIEW, shape);

        game->raiseEvent(event);
    }
}

constexpr BuildShape BuildSystem::getShape(const glm::ivec2& start, const glm::ivec2& end) {
    bool x = start.x == end.x;
    bool y = start.y == end.y;

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

std::vector<glm::ivec2> BuildSystem::getRoadNodes(const glm::ivec2& start, const glm::ivec2& end) const {
    BuildShape shape = getShape(start, end);
    if (shape == BuildShape::LINE) {
        return {start, end};
    }
    else {
        glm::ivec2 node = state.xFirst ? glm::ivec2(start.x, end.y) : glm::ivec2(end.x, start.y);
        return {start, node, end};
    }
}

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

constexpr glm::vec2 BuildSystem::getBuildmarkerOffset(const BuildingType type) {
    switch (type) {
        case BuildingType::LIFT_TERRAIN:
        case BuildingType::LOWER_TERRAIN:
            return glm::vec2(-0.5f);
        default:
            return glm::vec2(0.0f);
    }
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
        const BuildMarkerComponent& buildMarker = registry.get<BuildMarkerComponent>(buildMarkerEntity);

        BuildingType selectedType = state.selectedBuildingType;
        BuildAction action = BuildAction::DEFAULT;
        BuildShape shape = BuildShape::POINT;
        glm::ivec2 start;
        std::vector<glm::ivec2> positions = {buildMarker.position};

        state.currentPosition = buildMarker.position;

        bool canBuild;
        switch (selectedType) {
            case BuildingType::ROAD:
                action = state.building ? BuildAction::END : BuildAction::BEGIN;

                if (action == BuildAction::END) {
                    start = state.startPosition;

                    positions = getRoadNodes(start, buildMarker.position);
                }

                setState(selectedType, buildMarker.position, !state.building, buildMarker.position, state.xFirst);
                break;
            case BuildingType::PARKING_LOT:
                action = BuildAction::END;

                objectsToBuild.emplace(BuildInfo(resourceManager.getResource<Object>("object.parking_lot"), {buildMarker.position}, BuildingType::PARKING_LOT));
                break;
            default:
                break;
        }

        BuildEvent event = BuildEvent(positions, selectedType, action, shape);
#if DEBUG
        static std::string actionNames[] = {"DEFAULT", "BEGIN", "END"};

        std::cout << "Raising Build Event: {pos: (" << buildMarker.position << "), type: " << selectedType << ", action: " << actionNames[(unsigned int)action] << ", start: (" << start << ")}" << std::endl;
#endif
        game->raiseEvent(event);
    }
}

void BuildSystem::handleKeyEvent(const KeyEvent& e) {
    if (e.action == GLFW_PRESS) {
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
        else if (e.key == GLFW_KEY_X) {
            setState(this->state.selectedBuildingType, this->state.currentPosition, this->state.building, this->state.startPosition, !this->state.xFirst);
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

    setState(e.type, state.currentPosition, false);
}
