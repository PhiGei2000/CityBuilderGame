#pragma once
#include "misc/roads/graph.hpp"
#include "misc/roads/roadSpecs.hpp"
#include "misc/roads/roadTile.hpp"

#include <glm/glm.hpp>

#include <array>
#include <map>
#include <vector>

struct TerrainComponent;
struct RoadComponent;

class RoadPathGenerator {
  public:
    static RoadPath generateEdgePath(const RoadGraph::EdgeType& edge, const RoadComponent& roadComponent, const TerrainComponent& terrain, const std::map<std::string, RoadSpecs>& specs);

    static std::array<std::array<RoadPath, 4>, 4> generateNodePaths(const RoadGraph::NodeType& node, const RoadSpecs& specs, const RoadTile& tile, const TerrainComponent& terrain);
};
