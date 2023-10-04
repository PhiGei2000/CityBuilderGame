#pragma once
#include "../../misc/buildingType.hpp"
#include "../components/stackPanel.hpp"

class IconButton;
class Texture;

class BuildMenu : public StackPanel {
  private:
    Texture* liftTerrainButtonTexture;
    Texture* lowerTerrainButtonTexture;
    Texture* streetButtonTexture;
    Texture* parkingLotButtonTexture;

    IconButton* liftTerrainButton;
    IconButton* lowerTerrainButton;
    IconButton* streetButton;
    IconButton* parkingLotButton;

    void selectBuildingType(BuildingType type) const;

  public:
    BuildMenu(Gui* gui);
};
