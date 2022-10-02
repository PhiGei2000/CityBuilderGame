#pragma once
#include "components/component.hpp"
#include "misc/typedefs.hpp"

#include <typeindex>
#include <string>
#include <vector>

using ComponentTemplate = std::pair<std::type_index, Component>;

struct Object {
    std::string name;

    std::vector<ComponentTemplate> components;

    entt::entity create(entt::registry& registry) const;

protected:
    template<ComponentType T>
    static T& createComponent(const entt::entity& entity, entt::registry& registry, const T& component);
};

using ObjectPtr = ResourcePtr<Object>;
