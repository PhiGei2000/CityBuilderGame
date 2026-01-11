#pragma once
#include "components/component.hpp"
#include "gui/menus/buildMenu.hpp"
#include "misc/typedefs.hpp"

#include <glm/glm.hpp>

#include <string>
#include <typeindex>
#include <vector>

/// @brief This structure is a template for an entity that holds various components.
struct Object {
  protected:
    std::map<std::type_index, std::shared_ptr<AssignableComponent>> components;

  public:
    /// @brief The name of the object
    std::string name;

    /// @brief `True` if the object is buildable
    bool buildable = false;

    inline Object() {
    }

    inline Object(const std::string& name)
        : name(name) {
    }

    /// @brief Adds a template component to the object. The component type must be an assignable component.
    /// @tparam TComponent The type of the component
    /// @param component The component
    template<AssignableComponentType TComponent>
    inline void addComponent(const TComponent& component) {
        components[typeid(TComponent)] = std::make_shared<TComponent>(component);
    }

    /// @brief Creates an entity in the given registry and assings the associated components to this entity.
    /// @param registry The registry in which the component will be created
    /// @return The created entity
    inline entt::entity create(entt::registry& registry) const {
        const entt::entity entity = registry.create();

        for (const auto& [type, component] : components) {
            component->assignToEntity(entity, registry);
        }

        return entity;
    }
};

struct BuildableObject : public Object {
    struct BuildingInfo {
        glm::ivec2 defaultSize = glm::ivec2(1);
        glm::vec3 offset = glm::vec3(0.0f);
        std::string buildingID;
    } buildingInfo;

    BuildMenuEntry buildMenuEntry;

    inline BuildableObject(const std::string& buildingID) {
        buildingInfo.buildingID = buildingID;
        buildable = true;
    }

    inline BuildableObject(const std::string& name, const std::string& buildingID)
        : Object(name) {
        buildingInfo.buildingID = buildingID;
        buildable = true;
    }
};

/// @brief A resource pointer to an object
using ObjectPtr = ResourcePtr<Object>;
using BuildableObjectPtr = ResourcePtr<BuildableObject>;
