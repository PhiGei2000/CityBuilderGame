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
#include "misc/utility.hpp"
#include "misc/coordinateTransform.hpp"

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
    // update cars
    registry.view<CarComponent, TransformationComponent, VelocityComponent>().each(CarSystem::updateCar);

    // TODO: Implement path finding
}

void CarSystem::updateCar(CarComponent& car, TransformationComponent& transform, VelocityComponent& velocity) {
    if (car.driving) {
        if (car.currentPath.length() == 1 && car.positionOnPath > 1.0f) {
            car.driving = false;
            return;
        }

        const glm::vec3& pathSegmentStart = car.currentPath[0];
        const glm::vec3& pathSegmentEnd = car.currentPath[1];
        const glm::vec3& pathSegement = pathSegmentEnd - pathSegmentStart;

        const glm::vec3& posOnPath = transform.position - pathSegmentStart;

        car.positionOnPath = glm::dot(posOnPath, pathSegement) / (glm::length(posOnPath) * glm::length(pathSegement));

        if (car.positionOnPath >= 1.0f) {
            transform.setPosition(pathSegmentEnd);
            transform.calculateTransform();

            car.currentPath.removeFirst();
            car.positionOnPath = 0.0f;
            if (car.currentPath.length() >= 1.0f) {
                const glm::vec3& nextSegment = car.currentPath[1] - car.currentPath[0];

                const glm::vec3& rotationAxis = glm::normalize(glm::vec3(0.0f, nextSegment.z, -nextSegment.y)); //= glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), nextSegment)
                const float cosTheta = nextSegment.x / glm::length(nextSegment);

                const float sinThetaHalf = glm::sqrt(0.5f * (1 - cosTheta)); // theta in [0; pi]
                const float cosThetaHalf = glm::sqrt(0.5f * (1 + cosTheta)); // theta in [-pi; pi]

                transform.rotation = glm::quat(sinThetaHalf, cosThetaHalf * rotationAxis.x, cosThetaHalf * rotationAxis.y, cosThetaHalf * rotationAxis.z); // = e^(theta/2 * (ix + jy + kz))
            }
            else {
                car.driving = false;
                velocity.linearVelocity = glm::vec3(0);
            }
        }
    }
}

const entt::entity CarSystem::spawnCar(const glm::vec3& position, Direction drivingDirection) const {
    // float angle = -(int)drivingDirection * glm::radians(90.0f);

    // ObjectPtr car = resourceManager.getResource<Object>("object.car");

    // entt::entity entity = car->create(registry);
    // registry.emplace<TransformationComponent>(entity, position, glm::vec3(0.0f, angle, 0.0f), glm::vec3(1.0f));

    // return entity;

    return entt::entity();
}

void CarSystem::handleBuildEvent(BuildEvent& e) {
    if (registry.any_of<ParkingComponent>(e.entity)) {
        ParkingComponent& parkingComponent = registry.get<ParkingComponent>(e.entity);

        for (auto& parkingSpot : parkingComponent.parkingSpots) {
            if (parkingSpot.occupied) {
                spawnCar(parkingSpot.position, parkingSpot.direction);
            }
        }
    }
}
