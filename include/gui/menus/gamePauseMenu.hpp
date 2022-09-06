#pragma once
#include "../components/stackPanel.hpp"

class GamePauseMenu : public StackPanel {
  public:
    GamePauseMenu(Gui* gui);    

    void onResumeButtonClick(const MouseButtonEvent& e);
};
