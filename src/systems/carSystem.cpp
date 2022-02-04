#include "systems/carSystem.hpp"

#include "components/components.hpp"
#include "misc/configuration.hpp"
#include "misc/utility.hpp"
#include "resourceManager.hpp"

void CarSystem::init() {
    streetEntity = registry.view<StreetComponent>().front();

    for (int i = 0; i < Configuration::maxCars; i++) {
        spawnCar();
    }
}

void CarSystem::spawnCar() {
    const StreetComponent& street = registry.get<StreetComponent>(streetEntity);
    int edgesCount = street.graph.edges.size();

    if (edgesCount > 0 && cars.size() < Configuration::maxCars) {
        int direction = (rand() % 2) * 2 - 1;
        int edgeIndex = rand() % edgesCount;
        const StreetGraphEdge& edge = street.graph.edges[edgeIndex];

        // calculate coordinates
        float edgeOffset = rand() / static_cast<float>(RAND_MAX);
        glm::vec2 edgeVec = glm::vec2(edge.end - edge.start);
        glm::vec2 edgeDir = glm::normalize(edgeVec);

        glm::vec2 gridPos = edge.start;
        // move to edge position
        gridPos += edgeOffset * edgeVec;
        // move to edge center
        gridPos += glm::vec2(0.5f) - 0.5f * edgeDir;
        // move to right lane
        gridPos += (direction * 0.15f) * glm::vec2(edgeDir.y, -edgeDir.x);

        glm::vec3 pos = utility::toWorldCoords(gridPos) + glm::vec3(0, 0.1, 0);
        glm::mat3 rotation = glm::mat3(
            glm::vec3(edgeDir.x, 0, edgeDir.y),
            glm::vec3(0, 1, 0),
            glm::vec3(-edgeDir.y, 0, edgeDir.x));

        entt::entity car = registry.create();

        ResourceManager resourceManager = game->getResourceManager();
        registry.emplace<CarComponent>(car);
        registry.emplace<TransformationComponent>(car, pos, glm::toQuat(rotation), glm::vec3{1.0f}).calculateTransform();
        registry.emplace<MeshComponent>(car, resourceManager.getResource<Geometry>("CAR_GEOMETRY"), resourceManager.getResource<Shader>("MESH_SHADER"), resourceManager.getResource<Texture>("CAR_TEXTURE"));

        cars.push_back(car);
    }
}

CarSystem::CarSystem(Game* game)
    : System(game) {
    init();
}

void CarSystem::update(float dt) {
    spawnCar();
}
