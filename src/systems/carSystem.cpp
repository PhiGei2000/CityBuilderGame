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

        Direction drivingDir = misc::getDirection(edgeDir);
        const glm::vec2& destination = utility::toPlaneCoords(street.graph.getNextNode(gridPos, drivingDir).position) + StreetGraphNode::nodeInputs[static_cast<int>(drivingDir)];
        registry.emplace<CarComponent>(car,
                                       std::queue<glm::ivec2>(),
                                       drivingDir,
                                       destination,
                                       true,
                                       false);
        registry.emplace<TransformationComponent>(car, pos, glm::toQuat(rotation), glm::vec3{1.0f}).calculateTransform();
        registry.emplace<MeshComponent>(car, resourceManager.getResource<Geometry>("CAR_GEOMETRY"), resourceManager.getResource<Shader>("MESH_SHADER"), resourceManager.getResource<Texture>("CAR_TEXTURE"));

        registry.emplace<MovementComponent>(car, -static_cast<float>(direction) * glm::vec3(edgeDir.x, 0.0f, edgeDir.y), glm::vec3{0.0f});

        cars.push_back(car);
    }
}

glm::vec2 CarSystem::getDestination(Direction drivingDirection, const glm::ivec2& nodePos) {

    return static_cast<float>(Configuration::gridSize) * glm::vec2(nodePos) + StreetGraphNode::nodeInputs[static_cast<int>(drivingDirection)];
}

Direction CarSystem::getTurnEndDirection(Direction startDirection, Turns turn) {
    static constexpr int offsets[] = {
        /* Full */ 2,
        /* Left */ 3,
        /* Straight */ 0,
        /* Right */ 1};

    const int offset = offsets[static_cast<int>(turn)];
    return static_cast<Direction>((static_cast<int>(startDirection) + offset) % 4);
}

glm::vec2 CarSystem::getTurnDestination(Direction startDirection, const glm::vec2& startPos, Turns turn) {
    // determinate end direction
    Direction endDirection = getTurnEndDirection(startDirection, turn);

    // get input and output positions
    glm::vec2 input = StreetGraphNode::nodeInputs[static_cast<int>(startDirection)];
    glm::vec2 output = StreetGraphNode::nodeOutputs[static_cast<int>(endDirection)];

    // calculate destination position
    glm::vec2 diff = output - input;
    return startPos + diff;
}

std::vector<CarSystem::Turns> CarSystem::getPossibleTurns(Direction direction, const glm::vec2& position) const {
    const glm::ivec2& gridPosition = utility::toGridCoords(position);

    const StreetGraph& graph = registry.get<StreetComponent>(streetEntity).graph;
    auto it = graph.nodes.find(gridPosition);

    if (it != graph.nodes.end()) {
        int dir = static_cast<int>(direction);
        const StreetGraphNode& node = it->second;

        std::vector<Turns> turns;

        for (int i = 0; i < 4; i++) {
            int connection = (dir + i + 2) % 4;
            if (node.connections[connection]) {
                turns.push_back(static_cast<Turns>(i));
            }
        }

        return turns;
    }
    else
        return std::vector<Turns>{
            Turns::STAIGHT};
}

glm::vec3 CarSystem::getAngularVelocity(Turns turn) {
    // velocity is 1m/s or 3.6km/h
    static constexpr float velocity = 1.0f;

    // w = v/R ey
    switch (turn) {
    case Turns::FULL:
        return glm::vec3(0, 1 / 0.75f, 0);
    case Turns::LEFT:
        return glm::vec3(0, 1 / 3.75f, 0);
    case Turns::RIGHT:
        return glm::vec3(0, -1 / 1.75f, 0);
    default:
        return glm::vec3(0);
    }
}

glm::quat CarSystem::getCarRotation(Direction drivingDirection) {
    return glm::quat();
}

void CarSystem::handlePositionEvent(const PositionEvent& event) const {
    // check if entity has car component
    if (registry.all_of<CarComponent>(event.entity)) {
        CarComponent& car = registry.get<CarComponent>(event.entity);

        // set transform to exact position
        TransformationComponent& transform = registry.get<TransformationComponent>(event.entity);
        transform.position.x = event.position.x;
        transform.position.z = event.position.y;

        MovementComponent& movement = registry.get<MovementComponent>(event.entity);

        Turns turn = Turns::STAIGHT;
        // begin turn
        if (!car.inTurn) {
            std::vector<Turns> turns = getPossibleTurns(car.drivingDirection, event.position);

            turn = turns[(rand() % turns.size())];

            // update car component
            car.destination = getTurnDestination(car.drivingDirection, event.position, turn);
            car.inTurn = true;
            car.drivingDirection = getTurnEndDirection(car.drivingDirection, turn);
        }
        // drive to next node
        else {
            const StreetGraph& graph = registry.get<StreetComponent>(streetEntity).graph;

            // get node pos
            const glm::ivec2& gridPos = utility::toGridCoords(car.destination);
            const StreetGraphEdge& edge = graph.getEdge(gridPos, car.drivingDirection);

            const glm::ivec2& destinationNode = edge.start == gridPos ? edge.end : edge.start;
            car.destination = StreetGraphNode::nodeInputs[static_cast<int>(car.drivingDirection)] + utility::toPlaneCoords(destinationNode);
            car.inTurn = false;

            // update velocity and rotation
            const glm::vec2& velocity = DirectionVectors[car.drivingDirection];
            movement.linearVelocity = Configuration::carVelocity * glm::vec3(velocity.x, 0.0f, velocity.y);
            
            float angle = M_PI * (2 - 0.5f * static_cast<int>(car.drivingDirection));

            transform.setRotation(glm::vec3(0, 1, 0), angle);
        }

        // update angular velocity
        movement.angularVelocity = getAngularVelocity(turn);
    }
}

CarSystem::CarSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<PositionEvent>().connect<&CarSystem::handlePositionEvent>(*this);
}

void CarSystem::update(float dt) {
    spawnCar();

    const static float detectionRange = 0.1f;
    const StreetGraph& streetGraph = registry.get<StreetComponent>(streetEntity).graph;

    registry.view<CarComponent, TransformationComponent, MovementComponent>().each(
        [&](entt::entity entity, CarComponent& car, TransformationComponent& transform, MovementComponent& movement) {
            const glm::vec2& carPos = glm::vec2(transform.position.x, transform.position.z);
            if (car.driving) {
                // check if car has reached the end of the current path and raise event
                float dr = glm::length(carPos - car.destination);

                if (dr < detectionRange) {
                    const PositionEvent& e = PositionEvent{entity, car.destination};

                    eventDispatcher.trigger<PositionEvent>(e);
                }
            }
            else {
            }
        });
}
