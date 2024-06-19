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
#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/buildingType.hpp"
#include "misc/direction.hpp"

#include <queue>

#include <glm/glm.hpp>

struct MouseButtonEvent;
struct MouseMoveEvent;
struct KeyEvent;
struct TerrainComponent;
struct BuildingComponent;

struct Object;

class BuildSystem : public System {
  protected:
    // struct BuildInfo {
    // ObjectPtr object;
    // std::vector<glm::ivec2> positions;
    // Direction direction;

    // BuildingType type;

    // inline BuildInfo(ObjectPtr object, const std::vector<glm::ivec2>& positions, BuildingType type, Direction direction = Direction::UNDEFINED)
    //: object(object), positions(positions), type(type), direction(direction) {
    //}
    //};

    virtual void init() override;

    entt::entity cameraEntity;
    entt::entity currentBuilding;

    /// @brief Selected building type
    BuildingType selectedBuildingType = BuildingType::CLEAR;

    struct GridMouseIntersection {
        bool intersection;
        glm::ivec2 position;
        // glm::ivec2 lastPosition;
        bool positionChanged;

        bool positionUpdated() const;
    } gridMouseIntersection;

    /// @brief A list of objects that are going to be created
    std::queue<entt::entity> objectsToBuild;

    /// @brief Returns the current position of the mouse cursor projected to the grid
    /// @param mousePos Position of the mouse in screen coordinates
    /// @returns The position in normalized world grid coordinates
    std::pair<bool, glm::ivec2> getGridPos(const glm::vec2& mousePos, const glm::vec3& offset) const;

    /// @brief Returns the shape of the current building area according to the size vector
    /// @param start The start position of the building process
    /// @return The type of the shape of the building area
    static constexpr BuildShape getShape(const glm::ivec2& size);

    /// @brief Returns the default size of the building
    /// @param type The building type
    /// @returns The default size vector
    static constexpr glm::ivec2 getDefaultSize(BuildingType type);

    /// @brief Calculates the positions of road nodes based on start and end point of the road
    /// @param start The start position of the road
    /// @param end The end position of the road
    /// @returns The positions of the road nodes
    // std::vector<glm::ivec2> getRoadNodes(const glm::ivec2& start, const glm::ivec2& end) const;

    /// @brief Determines if the given building could be build
    /// @param positions The position data of the building
    /// @param type The building type
    /// @param terrain The terrain information
    /// @return True if the building could be build otherwise false
    bool canBuild(const std::vector<glm::ivec2>& positions, const BuildingType type, const TerrainComponent& terrain) const;

    static constexpr glm::vec3 getBuildingOffset(const BuildingType type);

    /// @brief Creates a new entity in the registry and assings the components for the currently selected building to this entity. In addition to these a BuildingComponent and a TransformationComponent
    void createNewBuilding();

    void updateMesh(const BuildingComponent& building) const;

  public:
    BuildSystem(Game* game);

    virtual void update(float dt) override;

    void handleMouseButtonEvent(const MouseButtonEvent& e);

    void handleMouseMoveEvent(const MouseMoveEvent& e);

    void handleKeyEvent(const KeyEvent& e);

    void handleBuildEvent(const BuildEvent& e);
};
