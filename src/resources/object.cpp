#include "resources/object.hpp"

#include "components/components.hpp"

entt::entity Object::create(entt::registry& registry) const {
    const entt::entity entity = registry.create();
    
    for (const auto& [type, component] : components) {
        if (type == typeid(BuildingComponent)) {
            createComponent<BuildingComponent>(entity, registry, static_cast<const BuildingComponent&>(component));
        }
        else if (type == typeid(LightComponent)) {
            createComponent<LightComponent>(entity, registry, static_cast<const LightComponent&>(component));
        }
        else if(type == typeid(MeshComponent)) {
            createComponent<MeshComponent>(entity, registry, static_cast<const MeshComponent&>(component));
        }
        else if (type == typeid(ParkingComponent)) {
            createComponent<ParkingComponent>(entity, registry, static_cast<const ParkingComponent&>(component));
        }
        else if(type == typeid(RoadComponent)) {
            createComponent<RoadComponent>(entity, registry, static_cast<const RoadComponent&>(component));
        }        
    }

    return entity;
}

template<ComponentType T>
T& Object::createComponent(const entt::entity& entity, entt::registry& registry, const T& component) {
    return registry.emplace<T>(entity, component);
}

template BuildingComponent& Object::createComponent<BuildingComponent>(const entt::entity&, entt::registry&, const BuildingComponent&);
// template BuildMarkerComponent& Object::createComponent<BuildMarkerComponent>(const entt::entity&, entt::registry&, const BuildMarkerComponent&);
// template CameraComponent& Object::createComponent<CameraComponent>(const entt::entity&, entt::registry&, const CameraComponent&);
// template CarComponent& Object::createComponent<CarComponent>(const entt::entity&, entt::registry&, const CarComponent&);
// template EnvironmentComponent& Object::createComponent<EnvironmentComponent>(const entt::entity&, entt::registry&, const EnvironmentComponent&);
template LightComponent& Object::createComponent<LightComponent>(const entt::entity&, entt::registry&, const LightComponent&);
template MeshComponent& Object::createComponent<MeshComponent>(const entt::entity&, entt::registry&, const MeshComponent&);
template ParkingComponent& Object::createComponent<ParkingComponent>(const entt::entity&, entt::registry&, const ParkingComponent&);
template RoadComponent& Object::createComponent<RoadComponent>(const entt::entity&, entt::registry&, const RoadComponent&);
// template TransformationComponent& Object::createComponent<TransformationComponent>(const entt::entity&, entt::registry&, const TransformationComponent&);
// template VelocityComponent& Object::createComponent<VelocityComponent>(const entt::entity&, entt::registry&, const VelocityComponent&);
