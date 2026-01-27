#pragma once
#include "gui/components/stackPanel.hpp"
#include "gui/menus/menu.hpp"

class GraphicsMenu : public Menu<StackPanel> {
  public:
    GraphicsMenu(Gui* gui);
};
