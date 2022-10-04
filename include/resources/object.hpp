#pragma once
#include "components/component.hpp"
#include "misc/typedefs.hpp"

#include <string>
#include <typeindex>
#include <vector>

struct Object {
  protected:
    std::map<std::type_index, std::shared_ptr<AssignableComponent>> components;

  public:
    std::string name;

    template<AssignableComponentType TComponent>
    void addComponent(const TComponent& component);

    entt::entity create(entt::registry& registry) const;
};

using ObjectPtr = ResourcePtr<Object>;
