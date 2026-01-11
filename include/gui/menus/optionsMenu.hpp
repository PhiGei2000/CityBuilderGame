#pragma once
#include "gui/components/stackPanel.hpp"
#include "gui/menus/menu.hpp"

class OptionsMenu : public Menu<StackPanel> {
  public:
    OptionsMenu(Gui* gui);
};
