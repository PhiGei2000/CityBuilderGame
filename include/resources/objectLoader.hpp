#pragma once
#include "object.hpp"

#include <concepts>

#include <pugixml.hpp>

class ResourceManager;

class ObjectLoader {
  protected:
    ResourceManager& resourceManager;

    template<ComponentType T> 
    T loadComponent(const pugi::xml_node& node);

  public:
    ObjectLoader(ResourceManager& resourceManager);

    ObjectPtr loadBuilding(const std::string& filename);
};
