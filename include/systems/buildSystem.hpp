#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/buildingType.hpp"
#include "misc/direction.hpp"

#include <queue>

#include <glm/glm.hpp>

struct MouseButtonEvent;
struct KeyEvent;

struct Object;

class BuildSystem : public System {
  protected:
    struct BuildInfo {
      ObjectPtr object;
      glm::ivec2 gridPosition;
      Direction direction;

      BuildingType type;
      glm::ivec2 startPosition;

      BuildInfo(ObjectPtr object, const glm::ivec2& gridPosition, Direction direction, BuildingType type);
      BuildInfo(ObjectPtr object, const glm::ivec2& gridPosition, Direction direction, BuildingType type, const glm::ivec2& startPosition);
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

    /// @brief sets the current state of the current building process
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

    std::vector<glm::ivec2> getRoadNodes(const glm::ivec2& start, const glm::ivec2& end) const;
  public:
    BuildSystem(Game* game);

    virtual void update(float dt) override;

    void handleMouseButtonEvent(const MouseButtonEvent& e);

    void handleKeyEvent(const KeyEvent& e);

    void handleBuildEvent(const BuildEvent& e);
};
