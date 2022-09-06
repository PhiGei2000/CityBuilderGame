#pragma once
#include "../components/stackPanel.hpp"

class PauseMenu : public StackPanel {
    void onResumeButtonClick(const MouseButtonEvent& e);
    void onOptionsButtonClick(const MouseButtonEvent& e);
    void onExitButtonClick(const MouseButtonEvent& e);

  public:
    PauseMenu(Gui* gui);
};
