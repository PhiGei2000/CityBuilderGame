#pragma once
#include "system.hpp"

#include "misc/roads/roadGraph.hpp"
#include "resources/roadPack.hpp"

#include <queue>
#include <tuple>

#include <glm/gtx/hash.hpp>

struct BuildEvent;
struct RoadComponent;
struct MeshGeometry;

class RoadSystem : public System {
  private:
    void init();

    std::queue<std::tuple<glm::ivec2, glm::ivec2>> sectionsToBuild;
    std::queue<std::tuple<glm::ivec2, glm::ivec2>> sectionsToPreview;

    RoadGraph previewGraph;

    entt::entity roadEntity;

  public:
    RoadSystem(Game* game);

    void update(float dt);
    
    void createRoadMesh(const RoadGraph& graph, MeshGeometry* geometry, ResourcePtr<RoadPack> roadPack) const;

    void clearRoadGraph(RoadGraph& graph) const;

    void handleBuildEvent(const BuildEvent& event);
};
