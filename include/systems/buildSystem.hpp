#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/buildingType.hpp"
#include "misc/direction.hpp"

#include <queue>

#include <glm/glm.hpp>

struct MouseButtonEvent;
struct KeyEvent;

class BuildSystem : public System {
  protected:
    struct BuildInfo {
      BuildingType type;
      glm::ivec2 gridPosition;
      Direction direction;
    };

    virtual void init() override;

    entt::entity cameraEntity;
    entt::entity buildMarkerEntity;

    struct BuildSystemState {
        /// @brief building process ongoing
        bool building = false;

        /// @brief Current position of the build marker
        glm::ivec2 currentPosition;

        /// @brief start position of the current building process
        glm::ivec2 startPosition;

        /// @brief selected building type
        BuildingType selectedBuildingType = BuildingType::CLEAR;

        //
        bool xFirst = true;
    };

    std::queue<BuildInfo> objectsToBuild;

    BuildSystemState state{};

    glm::ivec2 getGridPos(const glm::vec2& mousePos) const;

    void setState(BuildingType currentBuildingType, const glm::ivec2& currentPosition, bool building = false, const glm::ivec2& startPosition = glm::ivec2(-1), bool xFirst = true);

    static constexpr BuildShape getShape(const glm::ivec2& start, const glm::ivec2& end);

  public:
    BuildSystem(Game* game);

    virtual void update(float dt) override;

    void handleMouseButtonEvent(const MouseButtonEvent& e);

    void handleKeyEvent(const KeyEvent& e);

    void handleBuildEvent(const BuildEvent& e);
};
