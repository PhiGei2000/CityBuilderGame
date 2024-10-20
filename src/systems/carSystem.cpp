/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "systems/carSystem.hpp"

#include "components/components.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/utility.hpp"

static constexpr float rand_max = static_cast<float>(RAND_MAX);

void CarSystem::init() {
}

CarSystem::CarSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<BuildEvent>()
        .connect<&CarSystem::handleBuildEvent>(*this);
}

void CarSystem::update(float dt) {
    if (game->getState() != GameState::RUNNING) {
        return;
    }

    spawnCars();

    // update cars
    registry.view<CarComponent, TransformationComponent, VelocityComponent>().each(CarSystem::updateCar);

    // TODO: Implement path finding
    // registry.view<CarComponent>().each([&](CarComponent& car) {
    //     updateCarPath(car);
    // });
}

void CarSystem::updateCar(CarComponent& car, TransformationComponent& transform, VelocityComponent& velocity) {
    if (!car.driving) {
        return;
    }

    const auto& [pathSegmentStart, pathSegmentEnd, pathSegement] = car.currentPath.getCurrentSegment();

    const glm::vec3& posOnPath = transform.position - pathSegmentStart;

    car.positionOnPath = glm::length(posOnPath) / glm::length(pathSegement);
    std::cout << "Car position on path: " << car.positionOnPath << std::endl;

    if (car.positionOnPath >= 1.0f) {
        transform.setPosition(pathSegmentEnd);
        transform.calculateTransform();

        car.currentPath.removeFirst();
        car.positionOnPath = 0.0f;
    }

    if (car.currentPath.length() > 1) {
        const glm::vec3& nextSegment = car.currentPath[1] - car.currentPath[0];

        const glm::vec3& newX = glm::normalize(nextSegment);
        const glm::vec3& newZ = glm::cross(newX, glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::vec3& newY = glm::cross(newZ, newX);

        transform.rotation = glm::quat(glm::mat4(glm::vec4(newX, 0.0f), glm::vec4(newY, 0.0f), glm::vec4(newZ, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); // = e^(theta/2 * (ix + jy + kz))
        transform.calculateTransform();

        velocity.linearVelocity = Configuration::carVelocity * newX;
    }
    else {
        car.driving = false;
        velocity.linearVelocity = glm::vec3(0.0f);
    }
}

void CarSystem::updateCarPath(CarComponent& car) const {
    if (car.currentPath.length() != 2) {
        return;
    }

    const auto& [chunk, tilePos] = car.currentPath.getPathEndTile();
    if (!game->terrain.chunkLoaded(chunk)) {
        return;
    }

    const entt::entity chunkEntity = game->terrain.chunkEntities.at(chunk);
    const RoadGraph& graph = registry.get<RoadComponent>(chunkEntity).graph;
    const RoadGraph::NodeDataType& endNode = graph.getNodeData(tilePos);

    const auto [_, _, direction] = car.currentPath.getCurrentSegment();
    int incommingDirection = static_cast<int>(utility::getDirection(glm::vec2(-direction.x, -direction.z)));

    int outgoingDirecion = 0;
    while (outgoingDirecion < 4 && endNode.paths[incommingDirection][outgoingDirecion].length() == 0) {
        outgoingDirecion++;
    }

    if (outgoingDirecion > 3) {
        return;
    }

    car.currentPath.join(endNode.paths[incommingDirection][outgoingDirecion]);
}

void CarSystem::spawnCars() {
    if (game->getState() != GameState::RUNNING) {
        return;
    }

    const auto& loadedChunks = game->terrain.getLoadedChunks();
    if (loadedChunks.empty()) {
        return;
    }

    if (carsCount >= Configuration::maxCars) {
        return;
    }

    auto chunk = game->terrain.chunkEntities.at(loadedChunks[rand() % loadedChunks.size()]);
    const auto& roadComponent = registry.get<RoadComponent>(chunk);
    const auto& edges = roadComponent.graph.getEdges();
    if (edges.empty()) {
        return;
    }

    std::size_t edgeIndex = rand() % edges.size();
    auto nodeIt = edges.begin();
    std::advance(nodeIt, edgeIndex);

    const auto& [edge, path] = *nodeIt;
    if (path.length() == 0) {
        return;
    }

    spawnCar(path);
}

const entt::entity CarSystem::spawnCar(const RoadPath& path) {
    ObjectPtr car = resourceManager.getResource<Object>("object.Car");

    entt::entity entity = car->create(registry);
    CarComponent& carComponent = registry.get<CarComponent>(entity);
    carComponent.currentPath.join(path);
    carComponent.driving = true;

    registry.emplace<TransformationComponent>(entity, path[0], glm::vec3(0.0f), glm::vec3(1.0f));
    carsCount++;

    return entity;
}

const entt::entity CarSystem::spawnCar(const glm::vec3& position, float rotation) {
    ObjectPtr car = resourceManager.getResource<Object>("object.Car");

    entt::entity entity = car->create(registry);
    registry.emplace<TransformationComponent>(entity, position, glm::vec3(0.0f, rotation, 0.0f), glm::vec3(1.0f));

    carsCount++;
    return entity;
}

void CarSystem::handleBuildEvent(BuildEvent& e) {
    if (registry.any_of<ParkingComponent>(e.entity)) {
        ParkingComponent& parkingComponent = registry.get<ParkingComponent>(e.entity);

        for (auto& parkingSpot : parkingComponent.parkingSpots) {
            if (parkingSpot.occupied) {
                spawnCar(parkingSpot.position, -static_cast<int>(parkingSpot.direction) * glm::radians(90.0f));
            }
        }
    }
}
