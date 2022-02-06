#pragma once
#include "misc/configuration.hpp"
#include "rendering/geometry.hpp"
#include "streetGraph.hpp"

#include <map>

class ResourceManager;
struct StreetPack;

class StreetGeometryGenerator {
  private:
    ResourceManager& resourceManager;

    static constexpr glm::vec3 gridCenterOffset = static_cast<float>(Configuration::gridSize) * glm::vec3{0.5, 0, 0.5};

    GeometryData getNodeGeometry(const StreetGraphNode& node, const StreetPack& pack) const;
    GeometryData getEdgeGeometry(const StreetGraphEdge& edge, const StreetPack& pack) const;

  public:
    StreetGeometryGenerator(ResourceManager& resourceManager);

    Geometry* create(const StreetGraph& graph) const;

    static Geometry* createDebug(const StreetGraph& graph);
};
