#pragma once
#include "gui/components/stackPanel.hpp"
#include "gui/menus/menu.hpp"


class PauseMenu : public Menu<StackPanel> {
    void onResumeButtonClick(const MouseButtonEvent& e);
    void onOptionsButtonClick(const MouseButtonEvent& e);
    void onExitButtonClick(const MouseButtonEvent& e);

  public:
    PauseMenu(Gui* gui);

    inline constexpr virtual std::optional<int> getKey() const override {
        return GLFW_KEY_ESCAPE;
    }
};
