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
#include "resources/object.hpp"

#include "components/components.hpp"

template<AssignableComponentType T>
void Object::addComponent(const T& component) {    
    components[typeid(T)] = std::make_shared<T>(component);
}

entt::entity Object::create(entt::registry& registry) const {
    const entt::entity entity = registry.create();

    for (const auto& [type, component] : components) {        
        component->assignToEntity(entity, registry);
    }

    return entity;
}

template void Object::addComponent<BuildingComponent>(const BuildingComponent&);
template void Object::addComponent<CarComponent>(const CarComponent&);
template void Object::addComponent<CarPathComponent>(const CarPathComponent&);
template void Object::addComponent<EnvironmentComponent>(const EnvironmentComponent&);
template void Object::addComponent<LightComponent>(const LightComponent&);
template void Object::addComponent<MeshComponent>(const MeshComponent&);
template void Object::addComponent<ParkingComponent>(const ParkingComponent&);
template void Object::addComponent<RoadComponent>(const RoadComponent&);
template void Object::addComponent<TransformationComponent>(const TransformationComponent&);
template void Object::addComponent<VelocityComponent>(const VelocityComponent&);
