#include "systems/carSystem.hpp"

#include "components/components.hpp"
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
    bool carDriving = false;

    float threshold = 0.9999f;
    registry.view<ParkingComponent, BuildingComponent, CarPathComponent>().each(
        [&](ParkingComponent& parking, const BuildingComponent& building, const CarPathComponent& carPathComponent) {
            const glm::vec3& offset = utility::toWorldCoords(building.gridPosition);

            for (ParkingSpot& spot : parking.parkingSpots) {
                if (spot.occupied) {
                    if (rand() / rand_max > threshold) {                        
                        // get path
                        const CarPath& path = carPathComponent.paths.at(spot.id);

                        // set car path
                        const entt::entity carEntity = spot.entity;
                        CarComponent& car = registry.get<CarComponent>(carEntity);
                        car.currentPath = {};
                        for (const glm::vec3& pos : path.pathOut) {
                            car.currentPath.push(pos + offset);
                        }

                        car.lastPathPosition = car.currentPath.front();
                        car.currentPath.pop();
                        car.driving = true;

                        // set car velocity
                        const glm::vec3& direction = glm::normalize(car.currentPath.front() - car.lastPathPosition);
                        VelocityComponent& velocity = registry.get<VelocityComponent>(carEntity);

                        velocity.linearVelocity = 0.8f * direction;

                        // clear parking spot
                        spot.occupied = false;
                        spot.entity = entt::null;
                    }
                }
            }
        });

    // update driving cars
    registry.view<CarComponent, TransformationComponent, VelocityComponent>().each(updateCar);
}

void CarSystem::updateCar(CarComponent& car, TransformationComponent& transform, VelocityComponent& velocity) {
    if (car.driving) {
        float traveledDistance = glm::length(transform.position - car.lastPathPosition);
        float distanceToTravel = glm::length(car.currentPath.front() - car.lastPathPosition);

        // car has reached next path node
        if (traveledDistance >= distanceToTravel) {
            car.lastPathPosition = car.currentPath.front();
            transform.position = car.currentPath.front();

            car.currentPath.pop();
            // car has reached destination
            if (car.currentPath.size() == 0) {
                car.driving = false;
                velocity.linearVelocity = glm::vec3(0.0f);
                return;
            }

            glm::vec3 direction = glm::normalize(car.currentPath.front() - car.lastPathPosition);
            float yaw = glm::acos(direction.x);
            float pitch = glm::asin(direction.y);

            transform.setRotation(glm::vec3(pitch, yaw, 0.0f));
            transform.calculateTransform();

            velocity.linearVelocity = 2.0f * direction;
        }
    }
}

const entt::entity CarSystem::spawnCar(const glm::vec3& position, Direction drivingDirection) const {
    float angle = -(int)drivingDirection * glm::radians(90.0f);

    ObjectPtr car = resourceManager.getResource<Object>("object.car");

    entt::entity entity = car->create(registry);
    registry.emplace<TransformationComponent>(entity, position, glm::vec3(0.0f, angle, 0.0f), glm::vec3(1.0f));

    return entity;
}

void CarSystem::handleBuildEvent(BuildEvent& e) {
    static constexpr float threshold = 0.5f;

    if (e.type == BuildingType::PARKING_LOT && e.action == BuildAction::ENTITY_CREATED) {
        const glm::vec3& position = utility::toWorldCoords(e.gridPosition);

        registry.view<BuildingComponent, ParkingComponent>().each(
            [&](const BuildingComponent& building, ParkingComponent& parking) {
                if (e.gridPosition == building.gridPosition) {
                    for (ParkingSpot& parkingSpot : parking.parkingSpots) {
                        if (rand() / rand_max > threshold) {
                            entt::entity car = spawnCar(position + parkingSpot.position, Direction::NORTH);

                            parkingSpot.entity = car;
                            parkingSpot.occupied = true;
                        }
                    }
                }
            });
    }
}
