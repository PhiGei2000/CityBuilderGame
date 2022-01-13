#pragma once

#include "rendering/geometry.hpp"
#include "streetGraph.hpp"

#include <map>

class StreetGeometryGenerator {
private:
    static std::map<StreetGraphNodeType, GeometryData> nodeData;
    static GeometryData edgeData;

    static GeometryData getNodeGeometry(const StreetGraphNode& node);
    static GeometryData getEdgeGeometry(const StreetGraphEdge& edge);
public:
    static Geometry* create(const StreetGraph& graph);
}
