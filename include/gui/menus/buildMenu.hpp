#pragma once
#include "gui/components/stackPanel.hpp"
#include "gui/menus/menu.hpp"
#include "misc/buildingType.hpp"

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

class BuildMenu : public Menu<StackPanel> {
  private:
    Label* title;

    std::map<BuildingCategory, std::pair<Label*, StackPanel*>> categories;
    std::unordered_map<std::string, IconButton*> buttons;

    void selectBuildingType(const std::string& buildingID) const;

  public:
    BuildMenu(Gui* gui);
    ~BuildMenu();

    void addBuildingEntry(const BuildMenuEntry& menuEntry);

    inline virtual constexpr std::optional<int> getKey() const override {
        return GLFW_KEY_B;
    }

    inline virtual constexpr bool toggleOnKey() const override {
        return true;
    }
};
