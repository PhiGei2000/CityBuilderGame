#include "systems/roadSystem.hpp"

#include "components/meshComponent.hpp"
#include "components/roadComponent.hpp"
#include "components/transformationComponent.hpp"
#include "events/buildEvent.hpp"
#include "misc/configuration.hpp"
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

    Mesh* roadMesh = new Mesh(resourceManager.getResource<Shader>("MESH_SHADER"));
    roadMesh->geometries["BASIC_ROADS"]= {std::make_pair(resourceManager.getResource<Material>("BASIC_STREET_MATERIAL"), GeometryPtr(new MeshGeometry()))};
    roadMesh->geometries["BASIC_ROADS_PREVIEW"] = {std::make_pair(resourceManager.getResource<Material>("BASIC_STREET_PREVIEW_MATERIAL"), GeometryPtr(new MeshGeometry()))};

    registry.emplace<RoadComponent>(roadEntity);
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

        meshComponent.mesh->geometries.at("BASIC_ROADS_PREVIEW").front().second->fillBuffers({}, {});
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

        const glm::vec3& sectionPos = utility::toWorldCoords(node.position) + static_cast<float>(Configuration::gridSize) * glm::vec3(0.5f, 0.0f, 0.5f);

        int rotation = node.getRotation();
        glm::mat4 transform(1.0f);

        transform = glm::translate(transform, sectionPos);
        transform = glm::rotate(transform, rotation * glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        data.addData(nodeData.transformVertices(transform));

        // edges
        for (const auto& edge : node.edges) {
            if (edge.start == node.position) {
                Direction dir = utility::getDirection(edge.end - edge.start);

                bool rotate = utility::isEastWest(dir);
                const GeometryData& edgeData = pack->roadGeometries[RoadType::STRAIGHT];

                for (int i = 1; i < edge.length; i++) {
                    const glm::vec3& edgePos = utility::toWorldCoords(edge.start + i * DirectionVectors[dir]) + static_cast<float>(Configuration::gridSize) * glm::vec3(0.5f, 0.0f, 0.5f);

                    glm::mat4 edgeTransform(1.0f);
                    edgeTransform = glm::translate(edgeTransform, edgePos);

                    if (rotate) {
                        edgeTransform = glm::rotate(edgeTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    }

                    data.addData(edgeData.transformVertices(edgeTransform));
                }
            }
        }
    }

    geometry->fillBuffers(data);
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

    std::vector<std::pair<glm::ivec2, glm::ivec2>> sections;
    if (event.shape == BuildShape::LINE) {
        sections.emplace_back(event.startPosition, event.gridPosition);
    }
    else {
        glm::ivec2 p;
        if (event.xFirst) {
            p = glm::ivec2(event.gridPosition.x, event.startPosition.y);
        }
        else {
            p = glm::ivec2(event.startPosition.x, event.gridPosition.y);
        }

        sections.emplace_back(event.startPosition, p);
        sections.emplace_back(p, event.gridPosition);
    }

    if (event.action == BuildAction::END) {
        for (const auto& section : sections) {
            sectionsToBuild.push(section);
        }
    }
    else if (event.action == BuildAction::PREVIEW) {
        previewGraph.clear();

        for (const auto& section : sections) {
            sectionsToPreview.push(section);
        }
    }
}
