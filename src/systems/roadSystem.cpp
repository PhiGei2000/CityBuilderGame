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

    registry.emplace<RoadComponent>(roadEntity);
    registry.emplace<MeshComponent>(roadEntity, std::shared_ptr<Geometry>(new MeshGeometry()),
                                    resourceManager.getResource<Shader>("MESH_SHADER"),
                                    resourceManager.getResource<Material>("BASIC_STREET_MATERIAL"));
    registry.emplace<TransformationComponent>(roadEntity, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
}

void RoadSystem::update(float dt) {
    RoadComponent& roadComponent = registry.get<RoadComponent>(roadEntity);

    if (sectionsToBuild.size() > 0) {

        while (sectionsToBuild.size() > 0) {
            const auto& [start, end] = sectionsToBuild.front();

            const std::vector<RoadTile>& sections = createTiles(start, end);

            for (const auto& section : sections) {
                const glm::ivec2& pos = section.position;

                if (roadComponent.tiles.contains(pos)) {
                    roadComponent.tiles[pos].addConnections(section.connections);
                }
                else {
                    roadComponent.tiles.emplace(pos, section);
                }
            }

            sectionsToBuild.pop();
        }

        createRoadMesh();
    }
}

std::vector<RoadTile> RoadSystem::createTiles(const glm::ivec2& start, const glm::ivec2& end) const {
    const glm::ivec2& sectionVec = end - start;
    int length = glm::abs(sectionVec.x) + glm::abs(sectionVec.y);

    const glm::ivec2& directionVec = glm::normalize(sectionVec);
    Direction dir = utility::getDirection(directionVec);

    std::vector<RoadTile> sections;

    RoadTile::Connections connections = {false, false, false, false};
    // start tile
    connections[(unsigned int)dir] = true;
    sections.emplace_back(start, connections);

    // connection road section
    connections[((unsigned int)dir + 2) % 4] = true;
    for (int i = 1; i < length; i++) {
        sections.emplace_back(start + i * directionVec, connections);
    }

    // end tile
    connections[(unsigned int)dir] = false;
    sections.emplace_back(end, connections);

    return sections;
}

void RoadSystem::createRoadMesh() {
    RoadComponent& roadComponent = registry.get<RoadComponent>(roadEntity);
    MeshComponent& meshComponent = registry.get<MeshComponent>(roadEntity);

    GeometryData data{std::vector<Vertex>(), std::vector<unsigned int>()};

    std::shared_ptr<RoadPack> pack = resourceManager.getResource<RoadPack>("BASIC_STREETS");
    for (const auto& [_, tile] : roadComponent.tiles) {
        RoadType type = tile.getType();
        const GeometryData& sectionData = pack->roadGeometries[type];

        const glm::vec3& sectionPos = utility::toWorldCoords(tile.position) + static_cast<float>(Configuration::gridSize) * glm::vec3(0.5f, 0.0f, 0.5f);

        int rotation = tile.getRotation();
        glm::mat4 transform(1.0f);

        transform = glm::translate(transform, sectionPos);
        transform = glm::rotate(transform, rotation * glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        data.addData(sectionData.transformVertices(transform));
    }

    MeshGeometry* geometry = static_cast<MeshGeometry*>(meshComponent.geometry.get());
    geometry->fillBuffers(data);
}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    if (event.type != BuildingType::ROAD)
        return;

    if (event.action == BuildAction::END) {
        sectionsToBuild.emplace(event.buildingStartPosition, event.gridPosition);
    }
}
