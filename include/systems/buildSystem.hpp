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
struct CameraUpdateEvent;
struct GameStateChangedEvent;

struct Object;

class BuildSystem : public System {
  protected:
    virtual void init() override;

    entt::entity cameraEntity;
    entt::entity currentBuilding = entt::null;

    /// @brief Selected building type
    std::string selectedBuildingID = "";

    /// @brief True if the rotation of the building was modified
    bool buildingRotationUpdated = false;

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
    const glm::ivec2& getDefaultSize(std::string buildingID);

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
    bool canBuild(const std::vector<glm::ivec2>& positions, const std::string& buildingID) const;

    const glm::vec3 getBuildingOffset(const std::string& buildingID) const;

    /// @brief Creates a new entity in the registry and assings the components for the currently selected building to this entity. In addition to these a BuildingComponent and a TransformationComponent
    void createNewBuilding();

    void updateBuildingPosition(const BuildingComponent& building) const;

    void updateGridMouseIntersection();

    bool positionValid(const glm::ivec2& position) const;

  public:
    BuildSystem(Game* game);

    virtual void update(float dt) override;

    void handleMouseButtonEvent(const MouseButtonEvent& e);

    void handleMouseMoveEvent(const MouseMoveEvent& e);

    void handleCameraUpdateEvent(const CameraUpdateEvent& e);

    void handleKeyEvent(const KeyEvent& e);

    void handleBuildEvent(const BuildEvent& e);

    void handleBuildingSelectedEvent(const BuildingSelectedEvent& e);

    void handleGameStateChangedEvent(const GameStateChangedEvent& e);
};
