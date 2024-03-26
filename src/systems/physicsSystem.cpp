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
