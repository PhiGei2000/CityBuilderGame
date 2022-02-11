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

glm::vec2 CarSystem::getDestination(Direction drivingDirection, const glm::ivec2& nodePos) {
    static glm::vec2 nodeInputs[4] = {
        glm::vec2(4.25f, 0.0f),
        glm::vec2(0.0f, 1.75f),
        glm::vec2(1.75f, 5.0f),
        glm::vec2(5.0f, 4.25f)
    };

    return static_cast<float>(Configuration::gridSize) * glm::vec2(nodePos) + nodeInputs[static_cast<int>(drivingDirection)];
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
            if (car.path.size() > 0) {
                const glm::ivec2& nextNode = car.path.front();

                const glm::vec2& carPos = glm::vec2(transform.position.x, transform.position.z);
                if (car.driving) {
                    const glm::vec2& destination = getDestination(car.drivingDirection, nextNode);

                    // check if car has reached the end of the current path
                    if (glm::length(carPos - destination) < detectionRange) {
                        
                    }
                }
                else {                                    
                    
                }
            }
        });
}
