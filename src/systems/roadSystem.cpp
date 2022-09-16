#include "systems/roadSystem.hpp"

#include "components/meshComponent.hpp"
#include "components/roadComponent.hpp"
#include "components/transformationComponent.hpp"
#include "events/buildEvent.hpp"
#include "misc/configuration.hpp"
#include "misc/utility.hpp"
#include "resources/streetPack.hpp"

RoadSystem::RoadSystem(Game* game)
    : System(game) {

    eventDispatcher.sink<BuildEvent>()
        .connect<&RoadSystem::handleBuildEvent>(*this);

    init();
}

void RoadSystem::init() {
    roadEntity = registry.create();

    registry.emplace<RoadComponent>(roadEntity, std::unordered_map<glm::ivec2, RoadSection>());
    registry.emplace<MeshComponent>(roadEntity, std::shared_ptr<Geometry>(new MeshGeometry()),
                                    resourceManager.getResource<Shader>("MESH_SHADER"),
                                    resourceManager.getResource<Material>("BASIC_STREET_MATERIAL"));
    registry.emplace<TransformationComponent>(roadEntity, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
}

void RoadSystem::update(float dt) {
    RoadComponent& roadComponent = registry.get<RoadComponent>(roadEntity);

    if (sectionsToBuild.size() > 0) {

        while (sectionsToBuild.size() > 0) {
            const RoadSection& section = sectionsToBuild.front();

            const std::vector<RoadSection>& sections = createSection(section.start, section.end);

            for (const auto& roadSection : sections) {
                roadComponent.sections.emplace(roadSection.start, roadSection);
            }

            sectionsToBuild.pop();
        }

        createRoadMesh();
    }
}

std::vector<RoadSection> RoadSystem::createSection(const glm::ivec2& start, const glm::ivec2& end) const {
    const glm::ivec2& direction = glm::normalize(end - start);
    std::vector<RoadSection> sections;

    // start
    sections.emplace_back(start, start, RoadType::END);

    // connection
    sections.emplace_back(start + direction, end - direction, RoadType::EDGE);

    // end
    sections.emplace_back(end, end, RoadType::END);

    return sections;
}

void RoadSystem::createRoadMesh() {
    RoadComponent& roadComponent = registry.get<RoadComponent>(roadEntity);
    MeshComponent& meshComponent = registry.get<MeshComponent>(roadEntity);

    GeometryData data{std::vector<Vertex>(), std::vector<unsigned int>()};

    std::shared_ptr<StreetPack> pack = resourceManager.getResource<StreetPack>("BASIC_STREETS");
    for (const auto& [pos, section] : roadComponent.sections) {
        const GeometryData& sectionData = pack->streetGeometries[section.type];

        const glm::vec3& sectionPos = utility::toWorldCoords(section.start) + static_cast<float>(Configuration::gridSize) * glm::vec3(0.5f, 0.0f, 0.5f);
        if (section.type == RoadType::EDGE) {
            const glm::ivec2& sectionVec = glm::abs(section.end - section.start);
            const glm::ivec2& direction = glm::normalize(section.end - section.start);
            int length = glm::max(sectionVec.x, sectionVec.y);

            const glm::vec3& offset = utility::toWorldCoords(direction);
            for (int i = 0; i <= length; i++) {

                data.addData(GeometryData::transformVertices(sectionData, [&](const Vertex& vert) {
                    return Vertex(sectionPos + vert.position + static_cast<float>(i) * offset, vert.texCoord, vert.normal);
                }));
            }
        }
        else {
            data.addData(GeometryData::transformVertices(sectionData, [&](const Vertex& vert) {
                return Vertex(sectionPos + vert.position, vert.texCoord, vert.normal);
            }));
        }
    }

    MeshGeometry* geometry = static_cast<MeshGeometry*>(meshComponent.geometry.get());
    geometry->fillBuffers(data);
}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    if (event.type != BuildingType::ROAD)
        return;

    if (event.action == BuildAction::END) {
        // end of the road section
        sectionsToBuild.emplace(RoadSection{
            event.buildingStartPosition,
            event.gridPosition,
            RoadType::UNDEFINED});
    }
}
