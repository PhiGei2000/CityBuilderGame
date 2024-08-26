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
#include "object.hpp"
#include "gui/gui.hpp"

#include <concepts>
#include <unordered_map>
#include <string>

#include <pugixml.hpp>

class ResourceManager;
struct BuildMenuEntry;

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

    ObjectPtr loadObject(const std::string& filename, BuildMenuEntry* entry);
};
