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
#pragma once
#include "system.hpp"

#include "events/buildEvent.hpp"
#include "misc/direction.hpp"
#include "misc/roads/graph.hpp"

struct CarComponent;
struct TransformationComponent;
struct VelocityComponent;

class CarSystem : public System {
  protected:
    unsigned int carsCount = 0;
    void init() override;

    static void updateCar(CarComponent& carComponent, TransformationComponent& transform, VelocityComponent& velocity);
    void updateCarPath(CarComponent& carComponent) const;

    void spawnCars();

    const entt::entity spawnCar(const RoadPath& path);
    const entt::entity spawnCar(const glm::vec3& position, float roation);

  public:
    CarSystem(Game* game);

    void update(float dt) override;

    void handleBuildEvent(BuildEvent& e);
};
