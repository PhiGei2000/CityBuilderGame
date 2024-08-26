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
#include "../../misc/buildingType.hpp"
#include "../components/stackPanel.hpp"

class IconButton;
class Texture;
class Label;

struct BuildMenuEntry {
    std::string name;
    std::string section;
    BuildingCategory category;

    std::string buildingID;
    std::string iconFilename;
};

class BuildMenu : public StackPanel {
  private:
    Label* title;

    std::map<BuildingCategory, std::pair<Label*, StackPanel*>> categories;
    std::unordered_map<std::string, IconButton*> buttons;

    void selectBuildingType(const std::string& buildingID) const;

  public:
    BuildMenu(Gui* gui);
    ~BuildMenu();

    void addBuildingEntry(const BuildMenuEntry& menuEntry);
};
