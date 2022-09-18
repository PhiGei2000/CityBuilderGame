#include "systems/roadSystem.hpp"

#include "components/meshComponent.hpp"
#include "components/roadComponent.hpp"
#include "components/transformationComponent.hpp"
#include "events/buildEvent.hpp"
#include "misc/configuration.hpp"
#include "misc/direction.hpp"
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
            const auto& [start, end] = sectionsToBuild.front();

            const std::vector<RoadSection>& sections = createSection(start, end);

            for (const auto& roadSection : sections) {
                if (roadComponent.sections.contains(roadSection.position)) {
                    for (int i = 0; i < 4; i++) {
                        roadComponent.sections.at(roadSection.position).connections[i] |= roadSection.connections[i];
                    }
                }
                else {
                    roadComponent.sections.emplace(roadSection.position, roadSection);
                }
            }

            sectionsToBuild.pop();
        }

        createRoadMesh();
    }
}

std::vector<RoadSection> RoadSystem::createSection(const glm::ivec2& start, const glm::ivec2& end) const {
    const glm::ivec2& directionVec = glm::normalize(end - start);
    std::vector<RoadSection> sections;
    Direction dir = utility::getDirection(directionVec);

    // start tile
    std::array<bool, 4> startConnections({false, false, false, false});
    startConnections[(unsigned int)dir] = true;
    sections.emplace_back(start, startConnections);

    // connection road section
    sections.emplace_back(start + directionVec, end - directionVec);

    // end tile
    std::array<bool, 4> endConnections({false, false, false, false});
    endConnections[((unsigned int)dir + 2) % 4] = true;
    sections.emplace_back(end, endConnections);

    return sections;
}

void RoadSystem::createRoadMesh() {
    RoadComponent& roadComponent = registry.get<RoadComponent>(roadEntity);
    MeshComponent& meshComponent = registry.get<MeshComponent>(roadEntity);

    GeometryData data{std::vector<Vertex>(), std::vector<unsigned int>()};

    std::shared_ptr<StreetPack> pack = resourceManager.getResource<StreetPack>("BASIC_STREETS");
    for (const auto& [pos, section] : roadComponent.sections) {
        RoadType type = section.getType();
        const GeometryData& sectionData = pack->streetGeometries[type];

        const glm::vec3& sectionPos = utility::toWorldCoords(section.position) + static_cast<float>(Configuration::gridSize) * glm::vec3(0.5f, 0.0f, 0.5f);
        if (type == RoadType::EDGE) {

            const glm::ivec2& direction = glm::normalize(section.sectionVector);
            int length = section.getLength();
            bool rotate = direction.x == 0;

            const glm::vec3& offset = utility::toWorldCoords(direction);
            for (int i = 0; i <= length; i++) {

                glm::mat4 transform(1.0f);

                transform = glm::translate(transform, sectionPos + static_cast<float>(i) * offset);
                if (rotate) {
                    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                }

                data.addData(sectionData.transformVertices(transform));
            }
        }
        else {
            int rotation = section.getRotation();
            glm::mat4 transform(1.0f);

            transform = glm::translate(transform, sectionPos);
            transform = glm::rotate(transform, rotation * glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            data.addData(sectionData.transformVertices(transform));
        }
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
