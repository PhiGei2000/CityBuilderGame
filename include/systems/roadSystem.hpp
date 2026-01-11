#pragma once
#include "system.hpp"

#include "misc/roads/graph.hpp"
#include "resources/roadPack.hpp"

#include <queue>
#include <tuple>

#include <glm/gtx/hash.hpp>

struct BuildEvent;
struct RoadComponent;
struct MeshComponent;

class RoadSystem : public System {
  protected:
    std::queue<std::pair<glm::ivec2, std::string>> roadsToBuild;
    std::queue<glm::ivec2> chunksToUpdateMesh;

    std::map<std::string, RoadSpecs> roadSpecs;

    void createRoadMesh(const RoadComponent& roadComponent, MeshComponent& geometry, const glm::ivec2& chunk) const;

    void adjustTerrainHeight(const glm::ivec2& chunk, const glm::ivec2& position, RoadComponent& road) const;

    float getRoadHeight(const glm::ivec2& position, const RoadTile& tile) const;

    void init();

  public:
    RoadSystem(Game* game);

    void update(float dt);

    void handleBuildEvent(const BuildEvent& event);
};
