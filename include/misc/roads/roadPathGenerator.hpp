#pragma once
#include "misc/roads/graph.hpp"
#include "misc/roads/roadSpecs.hpp"
#include "misc/roads/roadTile.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <array>

class RoadPathGenerator {
  public:
    static RoadPath generateEdgePath(const RoadGraph::EdgeType& edge, const RoadSpecs& specs);

    static std::array<std::array<RoadPath, 4>, 4> generateNodePaths(const RoadGraph::NodeType& node, const RoadSpecs& specs, const RoadTile& tile);
};
