#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/buildingType.hpp"
#include "misc/direction.hpp"

#include <queue>

#include <glm/glm.hpp>

struct MouseButtonEvent;
struct KeyEvent;
struct TerrainComponent;

struct Object;

class BuildSystem : public System {
  protected:
    struct BuildInfo {
        ObjectPtr object;
        std::vector<glm::ivec2> positions;
        Direction direction;

        BuildingType type;

        inline BuildInfo(ObjectPtr object, const std::vector<glm::ivec2>& positions, BuildingType type, Direction direction = Direction::UNDEFINED)
            : object(object), positions(positions), type(type), direction(direction) {
        }
    };

    virtual void init() override;

    entt::entity cameraEntity;
    entt::entity buildMarkerEntity;

    /// @brief Structure that represents the state of the current building process
    struct BuildSystemState {
        /// @brief Building process ongoing
        bool building = false;

        /// @brief Current position of the build marker
        glm::ivec2 currentPosition;

        /// @brief Start position of the current building process
        glm::ivec2 startPosition;

        /// @brief Selected building type
        BuildingType selectedBuildingType = BuildingType::CLEAR;

        /// @brief Only used for road building. Describes if the road first goes to x-Direction or to y-Direction
        bool xFirst = true;
    } state;

    /// @brief A list of objects that are going to be created
    std::queue<BuildInfo> objectsToBuild;

    /// @brief Returns the current position of the mouse cursor projected to the grid
    glm::ivec2 getGridPos(const glm::vec2& mousePos) const;

    /// @brief Sets the current state of the current building process
    /// @param currentBuildingType The currently selected building type
    /// @param currentPosition The position of the build marker
    /// @param building True if the building process has been started
    /// @param startPosition The position where the building process has been started
    /// @param xFirst The direction of the building
    void setState(BuildingType currentBuildingType, const glm::ivec2& currentPosition, bool building = false, const glm::ivec2& startPosition = glm::ivec2(-1), bool xFirst = true);

    /// @brief Returns the shape of the current building area according to the start and current position
    /// @param start The start position of the building process
    /// @param end The current or end position of the build process
    /// @return The type of the shape of the building area
    static constexpr BuildShape getShape(const glm::ivec2& start, const glm::ivec2& end);

    /// @brief Calculates the positions of road nodes based on start and end point of the road
    /// @param start The start position of the road
    /// @param end The end position of the road
    /// @returns The positions of the road nodes
    std::vector<glm::ivec2> getRoadNodes(const glm::ivec2& start, const glm::ivec2& end) const;

    /// @brief Determines if the given building could be build
    /// @param positions The position data of the building
    /// @param type The building type
    /// @param terrain The terrain information
    /// @return True if the building could be build otherwise false
    bool canBuild(const std::vector<glm::ivec2>& positions, const BuildingType type, const TerrainComponent& terrain) const;

  public:
    BuildSystem(Game* game);

    virtual void update(float dt) override;

    void handleMouseButtonEvent(const MouseButtonEvent& e);

    void handleKeyEvent(const KeyEvent& e);

    void handleBuildEvent(const BuildEvent& e);
};
