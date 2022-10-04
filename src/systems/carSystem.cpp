#include "systems/carSystem.hpp"

#include "components/components.hpp"
#include "misc/utility.hpp"

void CarSystem::init() {
}

CarSystem::CarSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<BuildEvent>()
        .connect<&CarSystem::handleBuildEvent>(*this);
}

void CarSystem::update(float dt) {
}

const entt::entity CarSystem::spawnCar(const glm::vec3& position, Direction drivingDirection) const {   
    float angle = -(int)drivingDirection * glm::radians(90.0f);

    ObjectPtr car = resourceManager.getResource<Object>("object.car");

    entt::entity entity = car->create(registry);
    registry.emplace<TransformationComponent>(entity, position, glm::vec3(0.0f, angle, 0.0f), glm::vec3(1.0f));
    registry.emplace<VelocityComponent>(entity);

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
                        if (rand() / static_cast<float>(RAND_MAX) > threshold) {
                            entt::entity car = spawnCar(position + parkingSpot.position, Direction::NORTH);

                            parkingSpot.entity = car;
                            parkingSpot.occupied = true;
                        }
                    }
                }
            });
    }
}
