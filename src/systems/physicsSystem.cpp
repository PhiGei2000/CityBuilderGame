#include "systems/physicsSystem.hpp"

#include "components/components.hpp"

PhysicsSystem::PhysicsSystem(Game* game)
    : System(game) {
}

void PhysicsSystem::init() {
}

void PhysicsSystem::update(float dt) {
    registry.view<TransformationComponent, MovementComponent>().each(
        [&](TransformationComponent& transform, const MovementComponent& movement) {
            // apply translation
            // dr = v dt
            transform.position += movement.linearVelocity * dt;

            // apply rotation
            glm::quat dPhi = glm::angleAxis(glm::length(movement.angularVelocity) * dt, glm::normalize(movement.angularVelocity));
            transform.rotation *= dPhi;

            transform.calculateTransform();
        });
}
