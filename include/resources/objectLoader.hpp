#pragma once
#include "object.hpp"
#include "gui/gui.hpp"

#include <concepts>
#include <unordered_map>
#include <string>

#include <pugixml.hpp>

class ResourceManager;

class ObjectLoader {
  protected:
    ResourceManager& resourceManager;

    enum NodeNames {
      NAME,
      MESH,
      PARKING,
      CAR,
      PATH,
      VELCOITY,
      BUILDABLE
    };

    const static std::unordered_map<std::string, NodeNames> nodeNames;

    template<ComponentType T>
    T loadComponent(const pugi::xml_node& node);

    void loadBuildMenu(const pugi::xml_node& node) const;

  public:
    ObjectLoader(ResourceManager& resourceManager);

    ObjectPtr loadObject(const std::string& filename);
};
