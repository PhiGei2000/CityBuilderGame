#include "misc/streetGeometryGenerator.hpp"
#include "modelLoader.hpp"

std::map<StreetGraphNodeType, GeometryData> StreetGeometryGenerator::nodeData = {
    std::make_pair(StreetGraphNodeType::END, ModelLoader::load("res/models/street_end.obj")),
    std::make_pair(StreetGraphNodeType::CURVE, ModelLoader::load("res/models/street_curve.obj")),
    std::make_pair(StreetGraphNodeType::T_CROSSING, ModelLoader::load("res/models/street_t_crossing.obj")),
    std::make_pair(StreetGraphNodeType::CROSSING, ModelLoader::load("res/models/street_crossing.obj")),
};

GeometryData StreetGeometryGenerator::streetStraight = ModelLoader::load("res/models/street_straight.obj");

GeometryData StreetGeometryGenerator::getNodeGeometry(const StreetGraphNode& node) {

    int connectionsCount = (node.connections[0] ? 1 : 0) + (node.connections[1] ? 1 : 0) + (node.connections[2] ? 1 : 0) + (node.connections[3] ? 1 : 0);

    StreetGraphNodeType type = static_cast<StreetGraphNodeType>(connectionsCount);
    int rotation;

    switch (type) {
    case StreetGraphNodeType::END:
        // north connected
        if (node.connections[0]) {
            rotation = 0;
        }
        // east connected
        else if (node.connections[1]) {
            rotation = 1;
        }
        // south connected
        else if (node.connections[2]) {
            rotation = 2;
        }
        // west connected
        else if (node.connections[3]) {
            rotation = 3;
        }
        break;
    case StreetGraphNodeType::CURVE:
        // north connected
        if (node.connections[0]) {
            // east connected
            if (node.connections[1]) {
                rotation = 0;
            }
            // west connected
            else if (node.connections[3]) {
                rotation = 3;
            }
        }
        // south connected
        else if (node.connections[2]) {
            // east connected
            if (node.connections[1]) {
                rotation = 1;
            }
            // west connected
            else if (node.connections[3]) {
                rotation = 2;
            }
        }
        break;
    case StreetGraphNodeType::T_CROSSING:
        // north and south connected
        if (node.connections[0] && node.connections[2]) {
            // east connected
            if (node.connections[1]) {
                rotation = 0;
            }
            else if (node.connections[3]) {
                rotation = 2;
            }
        }
        // east and west connected
        if (node.connections[1] && node.connections[3]) {
            // north connected
            if (node.connections[0]) {
                rotation = 3;
            }
            // south connected
            else if (node.connections[2]) {
                rotation = 1;
            }
        }
        break;
    default:
        rotation = 0;
        break;
    }

    // get geometry
    GeometryData data = nodeData[type];

    // transform vertex positions
    const glm::vec3& worldCoords = toWorldCoords(node.position) + gridCenterOffset;
    float angle = glm::radians(rotation * 90.0f);

    const glm::mat3& rotationMat = glm::mat3{
        static_cast<float>(cos(angle)), static_cast<float>(0), static_cast<float>(sin(angle)),
        static_cast<float>(0), static_cast<float>(1), static_cast<float>(0),
        static_cast<float>(-sin(angle)), static_cast<float>(0), static_cast<float>(cos(angle))};

    for (Vertex& vert : data.vertices) {
        vert.position = worldCoords + rotationMat * vert.position;
    }

    // return transformed data
    return data;
}

constexpr glm::vec3 StreetGeometryGenerator::toWorldCoords(const glm::ivec2& gridCoords) {
    return static_cast<float>(Configuration::gridSize) * glm::vec3{gridCoords.x, 0, gridCoords.y};
}

GeometryData StreetGeometryGenerator::getEdgeGeometry(const StreetGraphEdge& edge) {
    // get edge length and direction
    glm::ivec2 diff = edge.end - edge.start;

    int edgeLength = abs(diff.x != 0 ? diff.x : diff.y);
    if (edgeLength == 0) {
        return GeometryData{};
    }

    glm::ivec2 directionVec = diff / edgeLength;

    bool horizontal = directionVec.y != 0;
    glm::vec3 beginOffset = toWorldCoords(edge.start) + gridCenterOffset;
    glm::vec3 stepOffset = toWorldCoords(directionVec);

    // transform vertex data for each grid cell and merge them together
    GeometryData data;

    if (horizontal) {

        const glm::mat3& rotation = glm::mat3{
            static_cast<float>(0), static_cast<float>(0), static_cast<float>(-1),
            static_cast<float>(0), static_cast<float>(1), static_cast<float>(0),
            static_cast<float>(1), static_cast<float>(0), static_cast<float>(0)};

        for (int i = 1; i < edgeLength; i++) {
            data.addData(GeometryData::transformVertices(streetStraight, [&](const Vertex& vert) {
                return Vertex{
                    rotation * vert.position + beginOffset + static_cast<float>(i) * stepOffset,
                    vert.texCoord,
                    vert.normal};
            }));
        }
    }
    else {
        for (int i = 1; i < edgeLength; i++) {
            data.addData(GeometryData::transformVertices(streetStraight, [&](const Vertex& vert) {
                return Vertex{
                    vert.position + beginOffset + static_cast<float>(i) * stepOffset,
                    vert.texCoord,
                    vert.normal};
            }));
        }
    }

    return data;
}

Geometry* StreetGeometryGenerator::create(const StreetGraph& graph) {
    GeometryData data;
    for (const auto& [position, node] : graph.nodes) {
        data.addData(getNodeGeometry(node));
    }

    for (const auto& edge : graph.edges) {
        data.addData(getEdgeGeometry(edge));
    }

    return new Geometry(data);
}
