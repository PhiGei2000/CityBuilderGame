#include "misc/streetGeometryGenerator.hpp"

#include "misc/configuration.hpp"
#include "modelLoader.hpp"

std::map<StreetGraphNodeType, GeometryData> StreetGeometryGenerator::nodeData = {
    std::make_pair(StreetGraphNodeType::END, ModelLoader::load("res/models/street_end.obj")),
    std::make_pair(StreetGraphNodeType::CURVE, ModelLoader::load("res/models/street_curve.obj")),
    std::make_pair(StreetGraphNodeType::T_CROSSING, ModelLoader::load("res/models/street_t_crossing.obj")),
    std::make_pair(StreetGraphNodeType::CROSSING, ModelLoader::load("res/models/street_crossing.obj")),
    std::make_pair(StreetGraphNodeType::EDGE_SEPARATOR, GeometryData{})};

GeometryData StreetGeometryGenerator::streetStraight = ModelLoader::load("res/models/street_straight.obj");

GeometryData StreetGeometryGenerator::getNodeGeometry(const StreetGraphNode& node) {
    GeometryData data;

    // get geometry
    if (node.type != StreetGraphNodeType::EDGE_SEPARATOR) {
        data = nodeData[node.type];
    }
    else {
        data = streetStraight;
    }

    // transform vertex positions
    const glm::vec3& worldCoords = toWorldCoords(node.position);
    for (Vertex& vert : data.vertices) {
        vert.position += worldCoords;
    }

    // return transformed data
    return data;
}

constexpr glm::vec3 StreetGeometryGenerator::toWorldCoords(const glm::ivec2& gridCoords) {
    return static_cast<float>(Configuration::gridSize) * glm::vec3{gridCoords.x, 0, gridCoords.y};
}

GeometryData StreetGeometryGenerator::getEdgeGeometry(const StreetGraphEdge& edge) {
    // get edge length and direction
    glm::ivec2 diff = edge.end - edge.begin;

    int edgeLength = diff.x != 0 ? diff.x : diff.y;
    glm::ivec2 directionVec = diff / edgeLength;

    int rotation = 0;
    glm::vec3 beginOffset = toWorldCoords(edge.begin);
    glm::vec3 stepOffset = toWorldCoords(directionVec);

    // transform vertex data for each grid cell and merge them together
    GeometryData data;

    for (int i = 0; i < edgeLength; i++) {
        data.addData(GeometryData::transformVertices(streetStraight, [&](const Vertex& vert) {
            return Vertex{
                vert.position + beginOffset + static_cast<float>(i) * stepOffset,
                vert.texCoord,
                vert.normal
            };
        }));
    }

    return data;
}

Geometry* StreetGeometryGenerator::create(const StreetGraph& graph) {
    GeometryData data;
    for (const auto& node : graph.nodes) {


        data.addData(getNodeGeometry(node));
    }

    for (const auto& edge : graph.edges) {
        data.addData(getEdgeGeometry(edge));
    }

    return new Geometry(data);
}
