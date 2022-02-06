#include "systems/carSystem.hpp"

#include "components/components.hpp"
#include "misc/carPath.hpp"
#include "misc/utility.hpp"
#include "resources/resourceManager.hpp"

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

        registry.emplace<CarComponent>(car);
        registry.emplace<TransformationComponent>(car, pos, glm::toQuat(rotation), glm::vec3{1.0f}).calculateTransform();
        registry.emplace<MeshComponent>(car, resourceManager.getResource<Geometry>("CAR_GEOMETRY"), resourceManager.getResource<Shader>("MESH_SHADER"), resourceManager.getResource<Texture>("CAR_TEXTURE"));

        registry.emplace<MovementComponent>(car, -static_cast<float>(direction) * glm::vec3(edgeDir.x, 0.0f, edgeDir.y), glm::vec3{0.0f});

        cars.push_back(car);
    }
}

CarSystem::CarSystem(Game* game)
    : System(game) {
    init();
}

void CarSystem::update(float dt) {
    spawnCar();

    const static float detectionRange = 0.1f;
    const StreetGraph& streetGraph = registry.get<StreetComponent>(streetEntity).graph;

    registry.view<CarComponent, TransformationComponent, MovementComponent>().each(
        [&](CarComponent& car, TransformationComponent& transform, MovementComponent& movement) {
            if (!car.turning) {
                const glm::vec2& carPos = glm::vec2(transform.position.x, transform.position.z);

                const glm::vec2& gridPos = utility::toGridCoords(transform.position);
                const glm::vec2& direction = glm::normalize(glm::vec2(movement.linearVelocity.x, movement.linearVelocity.z));

                auto it = streetGraph.nodes.find(gridPos);
                // car on node
                if (it != streetGraph.nodes.end()) {
                    const StreetGraphNode& node = it->second;

                    glm::vec2 localPos = carPos - static_cast<float>(Configuration::gridSize) * gridPos;

                    if (node.type == StreetType::END) {
                        const static glm::vec2 turnPoint = glm::vec2(1.75f, 2.5f);

                        if (glm::length(localPos - turnPoint) < detectionRange) {
                            // get start point
                            const glm::vec2& start = static_cast<float>(Configuration::gridSize) * gridPos + turnPoint;

                            // create car path

                            // set car position to start point
                            transform.position = glm::vec3(start.x, 0.1f, start.y);
                            // set angular velocity
                            movement.angularVelocity = glm::vec3(0.0f, 1 / 0.75f, 0.0f);
                            
                        }
                    }
                }
            }
        });
}
