#include "systems/carSystem.hpp"

#include "components/carComponent.hpp"
#include "components/meshComponent.hpp"
#include "components/transformationComponent.hpp"
#include "components/velocityComponent.hpp"

void CarSystem::init() {
}

CarSystem::CarSystem(Game* game)
    : System(game) {
    init();
}

void CarSystem::update(float dt) {
}

void CarSystem::spawnCar(const glm::vec3& position, Direction drivingDirection) const {
    auto entity = registry.create();

    float angle = -(int)drivingDirection * glm::radians(90.0f);

    registry.emplace<TransformationComponent>(entity, position, glm::vec3(0.0f, angle, 0.0f), glm::vec3(1.0f));
    registry.emplace<VelocityComponent>(entity);
    registry.emplace<MeshComponent>(entity, resourceManager.getResource<Geometry>("CAR_GEOMETRY"), resourceManager.getResource<Shader>("MESH_SHADER"), resourceManager.getResource<Material>("CAR_MATERIAL"));
    registry.emplace<CarComponent>(entity);
}
