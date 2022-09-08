#pragma once
#include "../../misc/buildingType.hpp"
#include "../components/stackPanel.hpp"

class IconButton;
class Texture;

class BuildMenu : public StackPanel {
  private:
    Texture* streetButtonTexture;
    IconButton* streetButton;

    void selectBuildingType(BuildingType type) const;

  public:
    BuildMenu(Gui* gui);
};
