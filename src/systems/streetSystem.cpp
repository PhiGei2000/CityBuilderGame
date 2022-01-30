#include "systems/streetSystem.hpp"

#include "components/components.hpp"
#include "events/events.hpp"

#include "misc/configuration.hpp"
#include "misc/streetGeometryGenerator.hpp"

StreetSystem::StreetSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<BuildEvent>()
        .connect<&StreetSystem::handle_buildEvent>(*this);
}

void StreetSystem::init() {
    const ResourceManager& resourceManager = game->getResourceManager();

    streetEntity = registry.create();
    registry.emplace<MeshComponent>(streetEntity, nullptr, resourceManager.getResource<Shader>("MESH_SHADER"), resourceManager.getResource<Texture>("STREET_TEXTURE"));
    registry.emplace<TransformationComponent>(streetEntity, glm::vec3{0}, glm::quat{}, glm::vec3{1});
    registry.emplace<StreetComponent>(streetEntity, StreetGraph{});
}

void StreetSystem::update(float dt) {
    StreetComponent& street = registry.get<StreetComponent>(streetEntity);

    if (streetGeometryOutdated) {
        MeshComponent& streetMesh = registry.get<MeshComponent>(streetEntity);

        street.graph.updateNodes();

        streetMesh.geometry.reset(StreetGeometryGenerator::create(street.graph));
        streetGeometryOutdated = false;

        entt::entity debugEntity = registry.view<DebugComponent>().front();
        registry.get<DebugComponent>(debugEntity).streetDebugMesh.geometry.reset(StreetGeometryGenerator::createDebug(street.graph));
    }

    while (buildData.size() > 0) {
        const StreetBuildData& data = buildData.front();

        street.graph.addEdge(data.start, data.end);
        buildData.pop();

        streetGeometryOutdated = true;
    }
}

void StreetSystem::handle_buildEvent(const BuildEvent& e) {
    if (e.type == BuildingType::STREET) {
        switch (e.buildType) {
        case BuildType::BEGIN:
            streetBuildStart = e.gridPosition;
            break;

        case BuildType::END:
            buildData.push({streetBuildStart, e.gridPosition});
        }
    }
}
