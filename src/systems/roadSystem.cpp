#include "systems/roadSystem.hpp"

#include "components/components.hpp"
#include "events/buildEvent.hpp"
#include "misc/configuration.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/direction.hpp"
#include "misc/utility.hpp"
#include "resources/roadPack.hpp"

RoadSystem::RoadSystem(Game* game)
    : System(game) {

    eventDispatcher.sink<BuildEvent>()
        .connect<&RoadSystem::handleBuildEvent>(*this);

    init();
}

void RoadSystem::init() {
    roadEntity = registry.create();

    Mesh* roadMesh = new Mesh();
    roadMesh->geometries["BASIC_ROADS"] = {std::make_pair(resourceManager.getResource<Material>("BASIC_STREET_MATERIAL"), GeometryPtr(new MeshGeometry()))};
    roadMesh->geometries["BASIC_ROADS_PREVIEW"] = {std::make_pair(resourceManager.getResource<Material>("BASIC_STREET_PREVIEW_MATERIAL"), GeometryPtr(new MeshGeometry()))};

    registry.emplace<RoadComponent>(roadEntity, RoadGraph());
    registry.emplace<TransformationComponent>(roadEntity, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    registry.emplace<MeshComponent>(roadEntity, MeshPtr(roadMesh));
}

void RoadSystem::update(float dt) {
    RoadComponent& roadComponent = registry.get<RoadComponent>(roadEntity);
    MeshComponent& meshComponent = registry.get<MeshComponent>(roadEntity);

    // sections to build
    if (sectionsToBuild.size() > 0) {
        while (sectionsToBuild.size() > 0) {
            const auto& [start, end] = sectionsToBuild.front();
            Direction dir = utility::getDirection(end - start);

            roadComponent.graph.insertNode(start);
            roadComponent.graph.insertNode(end);

            // connect nodes
            roadComponent.graph.connectNodes(start, end);

            sectionsToBuild.pop();
        }

        createRoadMesh(roadComponent.graph, reinterpret_cast<MeshGeometry*>(meshComponent.mesh->geometries.at("BASIC_ROADS").front().second.get()), resourceManager.getResource<RoadPack>("BASIC_STREETS"));

        meshComponent.mesh->geometries.at("BASIC_ROADS_PREVIEW").front().second->bufferData({}, {});
    }

    // sections to preview
    if (sectionsToPreview.size() > 0) {
        while (sectionsToPreview.size() > 0) {
            const auto& [start, end] = sectionsToPreview.front();

            Direction dir = utility::getDirection(end - start);
            previewGraph.insertNode(start);
            previewGraph.insertNode(end);

            for (Direction dir = Direction::NORTH; dir < Direction::UNDEFINED; dir++) {
                previewGraph.updateNodeConnection(end, dir);
                previewGraph.updateNodeConnection(start, dir);
            }

            sectionsToPreview.pop();
        }

        createRoadMesh(previewGraph, reinterpret_cast<MeshGeometry*>(meshComponent.mesh->geometries.at("BASIC_ROADS_PREVIEW").front().second.get()), resourceManager.getResource<RoadPack>("BASIC_STREETS_PREVIEW"));
    }
}

void RoadSystem::createRoadMesh(const RoadGraph& graph, MeshGeometry* geometry, ResourcePtr<RoadPack> pack) const {
    GeometryData data;
    for (const auto& [_, node] : graph.nodes) {
        // road node
        RoadType type = node.getType();
        const GeometryData& nodeData = pack->roadGeometries[type];

        glm::vec3 sectionPos = utility::gridToWorldCoords(node.position) + static_cast<float>(Configuration::cellSize) * glm::vec3(0.5f, 0.0f, 0.5f);
        sectionPos += glm::vec3(0.0f, game->terrain.getTerrainHeight(glm::vec2(node.position) + static_cast<float>(Configuration::cellSize) * glm::vec2(0.5f)), 0.0f);

        int rotation = node.getRotation();
        glm::mat4 transform(1.0f);

        transform = glm::translate(transform, sectionPos);
        transform = glm::rotate(transform, rotation * glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        data.addData(nodeData.transformVertices(transform));

        const GeometryData& edgeData = pack->roadGeometries[RoadType::STRAIGHT];
        const GeometryData& rampData = pack->roadGeometries[RoadType::RAMP];

        // edges
        for (const auto& edge : node.edges) {
            //             if (edge.start == node.position) {
            //                 Direction dir = utility::getDirection(edge.end - edge.start);

            //                 bool rotate = dir == Direction::WEST || dir == Direction::EAST;

            //                 for (int i = 1; i < edge.length; i++) {
            //                     const glm::ivec2 nodePos = edge.start + i * glm::ivec2(DirectionVectors[dir]);
            //                     glm::vec3 edgePos = utility::gridToWorldCoords(nodePos) + Configuration::cellSize * glm::vec3(0.5f, 0.0f, 0.5f);

            //                     // calculate transformation matrix
            //                     glm::mat4 edgeTransform(1.0f);

            //                     const TerrainSurfaceTypes surfaceType = game->terrain.getSurfaceType(glm::vec2(edgePos.x, edgePos.z));
            // #if DEBUG
            //                     std::cout << "terrain surface at: " << nodePos << " "
            //                               << surfaceType << std::endl;
            // #endif

            //                     switch (surfaceType) {
            //                         case TerrainSurfaceTypes::FLAT:
            //                             edgePos += glm::vec3(0.0f, terrain.getHeightValue(glm::vec2(edgePos.x, edgePos.z)), 0.0f);
            //                             edgeTransform = glm::translate(edgeTransform, edgePos);
            //                             if (rotate) {
            //                                 edgeTransform = glm::rotate(edgeTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //                             }

            //                             data.addData(edgeData.transformVertices(edgeTransform));
            //                             break;
            //                         case TerrainSurfaceTypes::DIAGONAL:
            //                             const glm::vec3 surfaceVector = terrain.getSurfaceVector(glm::vec2(edgePos.x, edgePos.z));
            //                             const glm::vec2 gradientDir = glm::normalize(glm::vec2(surfaceVector));

            //                             float rotation = glm::acos(gradientDir.x);
            //                             if (gradientDir.y > 0) {
            //                                 rotation *= -1;
            //                             }

            //                             edgePos += 2.0f * glm::floor(1 / 2.0f * glm::vec3(0.0f, terrain.getHeightValue(glm::vec2(edgePos.x, edgePos.z)), 0.0f));
            // #if DEBUG
            //                             std::cout
            //                                 << "rotating by angle: " << rotation << " rad" << std::endl;
            // #endif
            //                             edgeTransform = glm::translate(edgeTransform, edgePos);
            //                             edgeTransform = glm::rotate(edgeTransform, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            //                             data.addData(rampData.transformVertices(edgeTransform));
            //                             break;
            //                     }
            //                 }
            //             }
        }
    }

    geometry->bufferData(data);
}

void RoadSystem::clearRoadGraph(RoadGraph& graph) const {
    // remove edge nodes
    for (auto it = graph.nodes.begin(); it != graph.nodes.end();) {
        if (it->second.getType() == RoadType::STRAIGHT) {
            RoadGraphNode& node = it->second;
            glm::ivec2 start, end;

            if (node.getRotation() == 0) {
                start = node.edges[2].start != node.position ? node.edges[2].start : node.edges[2].end;
                end = node.edges[0].start != node.position ? node.edges[0].start : node.edges[0].end;

                graph.nodes[start].edges[0] = RoadGraphEdge(start, end);
                graph.nodes[end].edges[2] = RoadGraphEdge(start, end);
            }
            else {
                start = node.edges[3].start != node.position ? node.edges[3].start : node.edges[3].end;
                end = node.edges[1].start != node.position ? node.edges[1].start : node.edges[1].end;

                graph.nodes[start].edges[1] = RoadGraphEdge(start, end);
                graph.nodes[end].edges[3] = RoadGraphEdge(start, end);
            }

            it = graph.nodes.erase(it);
        }
    }
}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    if (event.type != BuildingType::ROAD)
        return;

    int segmentsCount = event.positions.size() - 1;

    previewGraph.clear();
    if (event.action == BuildAction::END) {
        for (int i = 0; i < segmentsCount; i++) {
            sectionsToBuild.emplace(event.positions[i], event.positions[i + 1]);
        }
    }
    else if (event.action == BuildAction::PREVIEW) {
        for (int i = 0; i < segmentsCount; i++) {
            sectionsToPreview.emplace(event.positions[i], event.positions[i + 1]);
        }
    }
}
