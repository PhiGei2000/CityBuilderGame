#include "systems/physicsSystem.hpp"

#include "components/components.hpp"

PhysicsSystem::PhysicsSystem(Game* game)
    : System(game) {
}

void PhysicsSystem::init() {
}

void PhysicsSystem::update(float dt) {
    registry.view<TransformationComponent, VelocityComponent>().each(
        [&](TransformationComponent& transform, VelocityComponent& movement) {
            // apply translation
            // dr = v dt
            transform.position += movement.linearVelocity * dt;

            // apply rotation
            if (glm::length(movement.angularVelocity) > 0) {
                glm::quat dPhi = glm::angleAxis(glm::length(movement.angularVelocity) * dt, glm::normalize(movement.angularVelocity));
                transform.rotation *= dPhi;

                glm::mat3 rotation = glm::toMat3(dPhi);
                movement.linearVelocity = rotation * movement.linearVelocity;
            }

            transform.calculateTransform();
        });
}
