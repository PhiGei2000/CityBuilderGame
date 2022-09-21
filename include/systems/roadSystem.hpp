#pragma once
#include "system.hpp"

#include <queue>
#include <tuple>

#include <glm/gtx/hash.hpp>

struct BuildEvent;
struct RoadTile;
struct RoadComponent;
struct MeshGeometry;

class RoadSystem : public System {
  private:
    struct NodeUpdateInfo {        
        std::array<bool, 4> connections = {false, false, false, false};
    };

    void init();

    std::queue<std::tuple<glm::ivec2, glm::ivec2>> sectionsToBuild;
    std::queue<std::tuple<glm::ivec2, glm::ivec2>> sectionsToPreview;

    std::unordered_map<glm::ivec2, NodeUpdateInfo> nodesChanged;

    entt::entity roadEntity;

  public:
    RoadSystem(Game* game);

    void update(float dt);

    std::vector<RoadTile> createTiles(const glm::ivec2& start, const glm::ivec2& end) const;

    void createRoadMesh();
    void createRoadMesh(std::unordered_map<glm::ivec2, RoadTile>& sections, MeshGeometry* geometry);

    void updateRoadGraph(RoadComponent& component);

    void handleBuildEvent(const BuildEvent& event);
};
